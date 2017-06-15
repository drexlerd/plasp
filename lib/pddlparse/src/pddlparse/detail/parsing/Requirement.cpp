#include <pddlparse/detail/parsing/Requirement.h>

#include <cstring>
#include <map>

#include <pddlparse/AST.h>
#include <pddlparse/ParserException.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Requirement
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct CompareStrings
{
	bool operator()(const char *lhs, const char *rhs) const
	{
		return std::strcmp(lhs, rhs) < 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////

using RequirementNameMap = std::map<const char *, ast::Requirement, CompareStrings>;
static const RequirementNameMap requirementNameMap =
	{
		{"strips", ast::Requirement::STRIPS},
		{"typing", ast::Requirement::Typing},
		{"negative-preconditions", ast::Requirement::NegativePreconditions},
		{"disjunctive-preconditions", ast::Requirement::DisjunctivePreconditions},
		{"equality", ast::Requirement::Equality},
		{"existential-preconditions", ast::Requirement::ExistentialPreconditions},
		{"universal-preconditions", ast::Requirement::UniversalPreconditions},
		{"quantified-preconditions", ast::Requirement::QuantifiedPreconditions},
		{"conditional-effects", ast::Requirement::ConditionalEffects},
		{"fluents", ast::Requirement::Fluents},
		{"numeric-fluents", ast::Requirement::NumericFluents},
		{"object-fluents", ast::Requirement::ObjectFluents},
		{"adl", ast::Requirement::ADL},
		{"durative-actions", ast::Requirement::DurativeActions},
		{"duration-inequalities", ast::Requirement::DurationInequalities},
		{"continuous-effects", ast::Requirement::ContinuousEffects},
		{"derived-predicates", ast::Requirement::DerivedPredicates},
		{"timed-initial-literals", ast::Requirement::TimedInitialLiterals},
		{"preferences", ast::Requirement::Preferences},
		{"constraints", ast::Requirement::Constraints},
		{"action-costs", ast::Requirement::ActionCosts},
		{"goal-utilities", ast::Requirement::GoalUtilities},
	};

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::Requirement parseRequirement(Tokenizer &tokenizer)
{
	const auto requirementName = tokenizer.getIdentifier();
	const auto matchingRequirement = requirementNameMap.find(requirementName.c_str());

	if (matchingRequirement == requirementNameMap.cend())
		throw ParserException(tokenizer.location(), "unknown PDDL requirement “" + requirementName + "”");

	return matchingRequirement->second;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

ast::Requirement parseRequirement(Context &context)
{
	auto &tokenizer = context.tokenizer;

	auto requirement = parseRequirement(tokenizer);

	if (requirement == ast::Requirement::GoalUtilities)
		context.warningCallback(tokenizer.location(), "requirement “goal-utilities” is not part of the PDDL 3.1 specification");

	return requirement;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const char *toString(const ast::Requirement &requirement)
{
	const auto matchingRequirement = std::find_if(requirementNameMap.cbegin(), requirementNameMap.cend(),
		[&](const auto &requirementNamePair)
		{
			return requirementNamePair.second == requirement;
		});

	assert(matchingRequirement != requirementNameMap.cend());

	return matchingRequirement->first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}