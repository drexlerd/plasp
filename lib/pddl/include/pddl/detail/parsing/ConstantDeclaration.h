#ifndef __PDDL__DETAIL__PARSING__CONSTANT_DECLARATION_H
#define __PDDL__DETAIL__PARSING__CONSTANT_DECLARATION_H

#include <pddl/ASTForward.h>
#include <pddl/Context.h>

namespace pddl
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ConstantDeclaration
//
////////////////////////////////////////////////////////////////////////////////////////////////////

void parseAndAddConstantDeclarations(Context &context, ast::Domain &domain);
void parseAndAddConstantDeclarations(Context &context, ast::Problem &problem);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}

#endif
