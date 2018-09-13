#pragma once

#include <cstdint>
#include <cstddef>

#include "rgxid.h"
#include "rgxss.h"

#include <z/core/array.h>

namespace z
{
	namespace util
	{
		/**
		 * \brief Regex pattern node object
		 *
		 * Regex patterns are parsed into a tree of nodes so that each node can simply call to match each of its children.
		 */
		class rgxll
		{
		private:
			rgxll* rgxParent;
			rgxll* rgxSibling;

			uint8_t rgxID;

			uint32_t symbolBeg;
			uint32_t symbolEnd;
			size_t minCount;
			size_t maxCount;

			bool isGreedy;

		public:
			///List of pointers to child nodes
			core::array<rgxll*> children;

			/**
			 * \brief Constructor directly from symbol ID
			 */
			rgxll(uint8_t ID);

			/**
			 * \brief Constructor from regexscan symbol
			 */
			rgxll(const rgxss& init);

			///Destructor
			~rgxll();

			/**
			 * \brief Get the ID of this node.
			 * \return The node ID.
			 */
			uint8_t id() const;

			/**
			 * \brief Get the starting character.
			 * \return The first character in the character range.
			 */
			uint32_t beg() const;

			/**
			 * \brief Get the ending character.
			 * \return The last character in the character range.
			 */
			uint32_t end() const;

			/**
			 * \brief Get the minimum times this symbol should match.
			 * \return The minimum symbol match count.
			 */
			size_t min() const;

			/**
			 * \brief Get the maximum times this symbol should match.
			 * \return The maximum symbol match count. (size_t)-1 if unlimited.
			 */
			size_t max() const;

			/**
			 * \brief Get whether this symbol is greedy in its matching priority.
			 * \return True if this symbol is greedy, false otherwise.
			 */
			bool greedy() const;

			/**
			 * \brief Set the ID of this node.
			 * \param newID The node ID.
			 */
			void setID(uint8_t newID);

			/**
			 * \brief Set the character range.
			 *
			 * \param beginSymbol The first character in the range.
			 * \param endSymbol The last character in the range.
			 */
			void setRange(uint32_t beginSymbol, uint32_t endSymbol);

			/**
			 * \brief Set to match a single character.
			 *
			 * This is equivalent to calling setRange() with beginSymbol equal to endSymbol.
			 *
			 * \param symbol The character to match.
			 */
			void setExact(uint32_t symbol);

			/**
			 * \brief Set to match no characters.
			 */
			void setNoChars();

			/**
			 * \brief Set to match this symbol a number of times within the given range.
			 *
			 * \param min The minimum times this symbol must match.
			 * \param max The maximum times this symbol may match. (size_t)-1 implies unlimited.
			 */
			void setCountRange(size_t min, size_t max);

			/**
			 * \brief Set to match this symbol an exact number of times.
			 *
			 * This symbol will only match if the count is exactly met, and
			 * will not consume extra input.
			 *
			 * \param count The number of times this symbol must match.
			 */
			void setCountExact(size_t count);

			/**
			 * \brief Set to match this symbol a minimum number of times.
			 *
			 * This symbol will only match if the count is met, but will
			 * consume extra input if it matches.
			 *
			 * \param count The minimum number of times this symbol must match.
			 */
			void setCountNoMax(size_t count);

			/**
			 * \brief Set the greediness of this symbol.
			 * \param status If true, the symbol will consume input greedily. If false, it will not.
			 */
			void setGreedy(bool status = true);

			/**
			 * \brief Get the parent of this node.
			 * \return A pointer to the parent node. NULL if no parent.
			 */
			rgxll* parent();

			/**
			 * \brief Get this node's sibling.
			 * \return a pointer to the next adjacent node. NULL if no nodes to the right of this one.
			 */
			rgxll* sibling();

			/**
			 * \brief Add a child to this node.
			 *
			 * Adds a child to this node's list of children. Also sets this node as that child's parent.
			 *
			 * \param node A pointer to the child node.
			 */
			void addChild(rgxll* node);

			/**
			 * \brief Set a node as this node's sibling.
			 *
			 * \param node A pointer to the next adjacent node.
			 */
			void setSibling(rgxll* node);
		};
	}
}
