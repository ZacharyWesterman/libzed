#pragma once
#include "rule.hpp"
#include <z/core/array.hpp>

namespace z
{
	namespace util
	{
		namespace rgx
		{
			/**
			 * \brief Intermediate class for regex rules that can have children.
			 */
			class compound : public rule
			{
			protected:
				/**
				 * \brief Attempt to match a single rule to the stream.
				 * \param rgxRule A pointer to the child rule to match.
				 * \param stream The stream to match against.
				 * \return True if the rule matches, false otherwise.
				 */
				bool matchRule(rule* rgxRule, core::inputStream& stream) const;

				/**
				 * \brief Attempt to match a rule its minimum number of times.
				 * \param rgxRule A pointer to the child rule to match.
				 * \param stream The stream to match against.
				 * \return True if the rule at least matches the minimum, false otherwise.
				 */
				bool matchMin(rule* rgxRule, core::inputStream& stream) const;

			public:
				/**
				 * \brief Compound destructor.
				 * Deletes any child rules.
				 */
				~compound();

				/**
				 * \brief Default full constructor.
				 * \param min The minimum number of times this rule must match.
				 * \param max The maximum number of times this rule can match.
				 * \param greedy Whether this rule should consume input greedily.
				 */
				compound(size_t min=1, size_t max=1, bool greedy=true): rule(min,max,greedy){}

				bool base() const {return false;};
				bool parent() const {return true;};

				///The list of child rules.
				core::array<rule*> children;
			};
		}
	}
}
