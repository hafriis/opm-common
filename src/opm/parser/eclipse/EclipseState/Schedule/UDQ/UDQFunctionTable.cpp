/*
  Copyright 2019 Statoil ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <functional>

#include <opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQFunctionTable.hpp>

namespace Opm {

UDQFunctionTable::UDQFunctionTable(UDQParams& params) :
    params(params)
{
    // SCalar functions
    this->insert_function( std::make_shared<UDQScalarFunction>("SUM", UDQScalarFunction::SUM) );
    this->insert_function( std::make_shared<UDQScalarFunction>("AVEA", UDQScalarFunction::AVEA) );
    this->insert_function( std::make_shared<UDQScalarFunction>("AVEG", UDQScalarFunction::AVEG) );
    this->insert_function( std::make_shared<UDQScalarFunction>("AVEH", UDQScalarFunction::AVEH) );
    this->insert_function( std::make_shared<UDQScalarFunction>("MAX", UDQScalarFunction::MAX) );
    this->insert_function( std::make_shared<UDQScalarFunction>("MIN", UDQScalarFunction::MIN) );
    this->insert_function( std::make_shared<UDQScalarFunction>("NORM1", UDQScalarFunction::NORM1) );
    this->insert_function( std::make_shared<UDQScalarFunction>("NORM2", UDQScalarFunction::NORM2) );
    this->insert_function( std::make_shared<UDQScalarFunction>("NORMI", UDQScalarFunction::NORMI) );
    this->insert_function( std::make_shared<UDQScalarFunction>("PROD", UDQScalarFunction::PROD) );

    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("ABS", UDQUnaryElementalFunction::ABS) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("DEF", UDQUnaryElementalFunction::DEF) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("EXP", UDQUnaryElementalFunction::EXP) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("IDV", UDQUnaryElementalFunction::IDV) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("LN", UDQUnaryElementalFunction::LN) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("LOG", UDQUnaryElementalFunction::LOG) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("NINT", UDQUnaryElementalFunction::NINT) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("SORTA", UDQUnaryElementalFunction::SORTA) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("SORTD", UDQUnaryElementalFunction::SORTD) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("UNDEF", UDQUnaryElementalFunction::UNDEF) );



    const auto& randn = [ &sim_rng = this->params.sim_rng() ](const UDQSet& arg) { return UDQUnaryElementalFunction::RANDN(sim_rng, arg); };
    const auto& randu = [ &sim_rng = this->params.sim_rng()] (const UDQSet& arg) { return UDQUnaryElementalFunction::RANDU(sim_rng, arg); };
    const auto& true_randn = [ &true_rng = this->params.true_rng() ](const UDQSet& arg) { return UDQUnaryElementalFunction::RANDN(true_rng, arg); };
    const auto& true_randu = [ &true_rng = this->params.true_rng() ](const UDQSet& arg) { return UDQUnaryElementalFunction::RANDU(true_rng, arg); };

    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("RANDN", randn) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("RANDU", randu) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("RRNDN", true_randn) );
    this->insert_function( std::make_shared<UDQUnaryElementalFunction>("RRNDU", true_randu) );

    const auto& eq = [ eps = this->params.cmpEpsilon()](const UDQSet&lhs, const UDQSet&rhs) { return UDQBinaryFunction::EQ(eps, lhs, rhs); };
    const auto& ne = [ eps = this->params.cmpEpsilon()](const UDQSet&lhs, const UDQSet&rhs) { return UDQBinaryFunction::NE(eps, lhs, rhs); };
    const auto& ge = [ eps = this->params.cmpEpsilon()](const UDQSet&lhs, const UDQSet&rhs) { return UDQBinaryFunction::GE(eps, lhs, rhs); };
    const auto& le = [ eps = this->params.cmpEpsilon()](const UDQSet&lhs, const UDQSet&rhs) { return UDQBinaryFunction::LE(eps, lhs, rhs); };

    this->insert_function( std::make_shared<UDQBinaryFunction>("==", eq) );
    this->insert_function( std::make_shared<UDQBinaryFunction>("!=", ne) );
    this->insert_function( std::make_shared<UDQBinaryFunction>(">=", ge) );
    this->insert_function( std::make_shared<UDQBinaryFunction>("<=", le) );

    this->insert_function( std::make_shared<UDQBinaryFunction>("^", UDQBinaryFunction::POW ));
    this->insert_function( std::make_shared<UDQBinaryFunction>("^", UDQBinaryFunction::POW ));
    this->insert_function( std::make_shared<UDQBinaryFunction>("<", UDQBinaryFunction::LT ));
    this->insert_function( std::make_shared<UDQBinaryFunction>(">", UDQBinaryFunction::GT ));
    this->insert_function( std::make_shared<UDQBinaryFunction>("+", UDQBinaryFunction::ADD ));
    this->insert_function( std::make_shared<UDQBinaryFunction>("*", UDQBinaryFunction::MUL ));
    this->insert_function( std::make_shared<UDQBinaryFunction>("/", UDQBinaryFunction::DIV ));
    this->insert_function( std::make_shared<UDQBinaryFunction>("-", UDQBinaryFunction::SUB ));

    this->insert_function( std::make_shared<UDQBinaryFunction>("UADD", UDQBinaryFunction::UADD ));
    this->insert_function( std::make_shared<UDQBinaryFunction>("UMUL", UDQBinaryFunction::UMUL ));
    this->insert_function( std::make_shared<UDQBinaryFunction>("UMIN", UDQBinaryFunction::UMIN ));
    this->insert_function( std::make_shared<UDQBinaryFunction>("UMAX", UDQBinaryFunction::UMAX ));
}


void UDQFunctionTable::insert_function(std::shared_ptr<const UDQFunction> func) {
    auto name = func->name();
    this->function_table.emplace( std::move(name), std::move(func) );
}



bool UDQFunctionTable::has_function(const std::string& name) const {
    return this->function_table.count(name) > 0;
}


const UDQFunction& UDQFunctionTable::get(const std::string& name) const {
    if (!this->has_function(name))
        throw std::invalid_argument("No such function registered: " + name);

    const auto& pair_ptr = this->function_table.find(name);
    return *pair_ptr->second;
}
}
