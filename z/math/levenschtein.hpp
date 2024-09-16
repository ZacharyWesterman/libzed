#pragma once

#include "../core/string.hpp"

namespace z
{
	namespace math
	{
		/**
		* @brief Computes the Levenshtein distance between two strings with an optional maximum threshold.
		*
		* This function calculates the Levenshtein distance, which is a metric for measuring the difference between
		* two strings `word1` and `word2`. It represents the minimum number of single-character edits (insertions, 
		* deletions, or substitutions) required to change one string into the other.
		*
		* The function allows for an optional maximum distance `max`. If the calculated Levenshtein distance exceeds
		* this threshold, it will return early with the maximum possible value, as the strings are -- for all intents
		* and purposes -- completely different.
		*
		* \param word1 The first string to compare.
		* \param word2 The second string to compare.
		* \param max The optional maximum distance threshold. If the Levenshtein distance exceeds this value, the maximum integer value is returned.
		* @return The Levenshtein distance between the two strings.
		*/
		template<encoding E>
		int levenschtein(const z::core::string<E>& word1, const z::core::string<E>& word2, int max = 16) noexcept;
	}
}
