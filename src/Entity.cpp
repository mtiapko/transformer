#include <limits>
#include "transformer/TokenPtr.h"
#include "transformer/Entity.h"
#include "transformer/Util.h"

namespace transformer
{

Entity::Entity(CXCursor cur) noexcept
	: Entity(cur, Util::get_full_name(cur))
{}

Entity::Entity(CXCursor cur, std::string full_name) noexcept
	: m_is_from_main_file { (clang_Location_isFromMainFile(clang_getCursorLocation(cur)) != 0) }
	, m_file_path         { Util::get_file_path(cur) }
	, m_full_name         { std::move(full_name) }
	, m_name              { Util::to_string(clang_getCursorSpelling(cur)) }
{}

// TODO(FiTH): clang tokens API is garbage. might be bugs in parsing. replace it?
CXSourceLocation Entity::skip_current_token(const CXTranslationUnit tu,
	const CXSourceLocation& current_loc, int32_t step) noexcept
{
	CXFile file;
	uint32_t current_offset;
	clang_getSpellingLocation(current_loc, &file, nullptr, nullptr, &current_offset);

	int32_t offset = 0;
	while (true) {
		CXSourceLocation loc = clang_getLocationForOffset(tu, file, current_offset + offset);
		TokenPtr token { tu, clang_getToken(tu, loc), 1 };

		if (token == nullptr)
			break;

		CXSourceRange token_range = clang_getTokenExtent(tu, *token);

		CXSourceLocation token_range_side;
		if (step < 0) token_range_side = clang_getRangeStart(token_range);
		else token_range_side = clang_getRangeEnd(token_range);

		if (clang_equalLocations(token_range_side, current_loc) == 0) {

		if (step > 0) token_range_side = clang_getRangeStart(token_range);
		else token_range_side = clang_getRangeEnd(token_range);
			return token_range_side;
		}

		offset += step;
	}

	return clang_getNullLocation();
}

CXSourceLocation Entity::find_double_char_location(const CXTranslationUnit& tu,
	CXSourceLocation current_loc, std::string_view char_str, int32_t offset, bool force) noexcept
{
	CXFile file;
	uint32_t current_offset;
	clang_getSpellingLocation(current_loc, &file, nullptr, nullptr, &current_offset);

	CXSourceLocation last_loc = current_loc;
	bool first_already_found = false;
	int32_t step = (offset < 0 ? -1 : 1);
	while (true) {
		CXSourceLocation loc = clang_getLocationForOffset(tu, file, current_offset + offset);
		offset += step;

		TokenPtr token { tu, clang_getToken(tu, loc), 1 };
		if (token == nullptr)
			break;

		CXSourceRange token_range = clang_getTokenExtent(tu, *token);

		CXSourceLocation token_range_side;
		if (step < 0) token_range_side = clang_getRangeStart(token_range);
		else token_range_side = clang_getRangeEnd(token_range);

		if (clang_equalLocations(token_range_side, current_loc) == 0) {
			current_loc = token_range_side;

			if (clang_getTokenKind(*token) != CXToken_Punctuation) {
				if (!force)
					break;

				first_already_found = false;
			} else {
				auto token_str = Util::to_string(clang_getTokenSpelling(tu, *token));
				if (token_str != char_str) {
					if (!force)
						break;

					first_already_found = false;
				} else {
					if (first_already_found) {
						if (force)
							return last_loc;

						/*
							parsing from right to left ( <- )

							[[ ... attrib ... ]]
							skip this        _^_
						*/
						if (step < 0)
							return skip_current_token(tu, token_range_side, step);

						return token_range_side;
					}

					first_already_found = true;
					continue;
				}
			}
		} else {
			if (step < 0)
				first_already_found = false;
			else
				continue;
		}

		last_loc = current_loc;
	}

	return clang_getNullLocation();
}

void Entity::add_attribute(std::string_view attribute) noexcept
{
	if (attribute.empty())
		return;

	if (!m_attributes.empty())
		m_attributes += ' '; // TODO(FiTH): or ", "?

	m_attributes += attribute;
}

/* static */ std::string Entity::parse_attributes(const CXTranslationUnit& tu,
	const CXSourceLocation& loc, int32_t offset) noexcept
{
	CXSourceLocation end = clang_getNullLocation();
	CXSourceLocation beg = clang_getNullLocation();

	if (offset < 0) {
		end = find_double_char_location(tu, loc, "]", offset, false);
		if (clang_equalLocations(end, clang_getNullLocation()))
			return {};

		beg = find_double_char_location(tu, end, "[", offset, true);
		if (clang_equalLocations(beg, clang_getNullLocation()))
			return {};
	} else {
		beg = find_double_char_location(tu, loc, "[", offset, false);
		if (clang_equalLocations(beg, clang_getNullLocation()))
			return {};

		end = find_double_char_location(tu, beg, "]", 0, true);
		if (clang_equalLocations(end, clang_getNullLocation()))
			return {};
	}

	CXSourceRange tokens_range = clang_getRange(beg, end);

	CXToken* cxtokens;
	uint32_t cxtokens_count;
	clang_tokenize(tu, tokens_range, &cxtokens, &cxtokens_count);

	TokenPtr tokens { tu, cxtokens, cxtokens_count };

	std::string attribute;
	uint32_t cur_offset = std::numeric_limits<uint32_t>::max();
	for (uint32_t i = 0; i < tokens.count(); ++i) {
		CXSourceLocation cur_tok_loc = clang_getRangeStart(clang_getTokenExtent(tu, tokens[i]));

		uint32_t offset;
		clang_getSpellingLocation(cur_tok_loc, nullptr, nullptr, nullptr, &offset);

		if (offset > cur_offset)
			attribute += ' ';

		cur_tok_loc = clang_getRangeEnd(clang_getTokenExtent(tu, tokens[i]));
		clang_getSpellingLocation(cur_tok_loc, nullptr, nullptr, nullptr, &offset);

		cur_offset = offset;

		attribute += Util::to_string(clang_getTokenSpelling(tu, tokens[i]));
	}

	return attribute;
}

}
