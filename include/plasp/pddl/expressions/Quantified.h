#ifndef __PLASP__PDDL__EXPRESSIONS__QUANTIFIED_H
#define __PLASP__PDDL__EXPRESSIONS__QUANTIFIED_H

#include <plasp/pddl/Context.h>
#include <plasp/pddl/Expression.h>
#include <plasp/pddl/ExpressionContext.h>
#include <plasp/pddl/expressions/Variable.h>

namespace plasp
{
namespace pddl
{
namespace expressions
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Quantified
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
class Quantified: public ExpressionCRTP<Derived>
{
	public:
		template<typename ExpressionParser>
		static boost::intrusive_ptr<Derived> parse(Context &context,
			ExpressionContext &expressionContext, ExpressionParser parseExpression);

	public:
		void setArgument(ExpressionPointer argument);
		ExpressionPointer argument() const;

		Variables &variables();
		const Variables &variables() const;

		ExpressionPointer reduced() override;
		ExpressionPointer negationNormalized() override;
		ExpressionPointer prenex() override;
		ExpressionPointer simplified() override;

		void print(std::ostream &ostream) const override;

	protected:
		Variables m_variables;
		ExpressionPointer m_argument;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
template<typename ExpressionParser>
boost::intrusive_ptr<Derived> Quantified<Derived>::parse(Context &context,
	ExpressionContext &expressionContext, ExpressionParser parseExpression)
{
	auto &parser = context.parser;

	const auto position = parser.position();

	if (!parser.testAndSkip<std::string>("(")
		|| !parser.testIdentifierAndSkip(Derived::Identifier))
	{
		parser.seek(position);
		return nullptr;
	}

	auto expression = boost::intrusive_ptr<Derived>(new Derived);

	// Parse variable list
	parser.expect<std::string>("(");
	Variable::parseTypedDeclarations(context, expressionContext, expression->m_variables);
	parser.expect<std::string>(")");

	// Push newly parsed variables to the stack
	expressionContext.variables.push(&expression->m_variables);

	// Parse argument of the expression
	expression->Quantified<Derived>::setArgument(parseExpression(context, expressionContext));

	// Clean up variable stack
	expressionContext.variables.pop();

	parser.expect<std::string>(")");

	return expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
void Quantified<Derived>::setArgument(ExpressionPointer expression)
{
	m_argument = expression;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
ExpressionPointer Quantified<Derived>::argument() const
{
	return m_argument;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
Variables &Quantified<Derived>::variables()
{
	return m_variables;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
const Variables &Quantified<Derived>::variables() const
{
	return m_variables;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer Quantified<Derived>::reduced()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->prenex();

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer Quantified<Derived>::negationNormalized()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->prenex();

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer Quantified<Derived>::prenex()
{
	BOOST_ASSERT(m_argument);

	// Quantifiers may not move before other quantifiers, their order matters
	m_argument = m_argument->prenex();

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline ExpressionPointer Quantified<Derived>::simplified()
{
	BOOST_ASSERT(m_argument);

	m_argument = m_argument->simplified();

	// Associate same-type children, such as (forall (?x) (forall (?y) (...)))
	if (m_argument->expressionType() != Derived::ExpressionType)
		return this;

	auto &quantifiedExpression = dynamic_cast<Derived &>(*m_argument);

	// Unify variables
	m_variables.insert(m_variables.end(), quantifiedExpression.variables().begin(), quantifiedExpression.variables().end());

	// Move child expression up
	m_argument = quantifiedExpression.argument();

	// TODO: introduce/handle boolean values

	return this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
inline void Quantified<Derived>::print(std::ostream &ostream) const
{
	ostream << "(" << Derived::Identifier << " (";

	for (size_t i = 0; i < m_variables.size(); i++)
	{
		if (i > 0)
			ostream << " ";

		m_variables[i]->print(ostream);
	}

	ostream << ") ";

	m_argument->print(ostream);

	ostream << ")";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
