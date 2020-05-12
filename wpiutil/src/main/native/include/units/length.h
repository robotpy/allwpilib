#include "base.h"

	//------------------------------
	//	LENGTH UNITS
	//------------------------------

	/**
	 * @namespace	units::length
	 * @brief		namespace for unit types and containers representing length values
	 * @details		The SI unit for length is `meters`, and the corresponding `base_unit` category is
	 *				`length_unit`.
	 * @anchor		lengthContainers
	 * @sa			See unit_t for more information on unit type containers.
	 */
#if !defined(DISABLE_PREDEFINED_UNITS) || defined(ENABLE_PREDEFINED_LENGTH_UNITS)
	UNIT_ADD_WITH_METRIC_PREFIXES(length, meter, meters, m, unit<std::ratio<1>, units::category::length_unit>)
	UNIT_ADD(length, foot, feet, ft, unit<std::ratio<381, 1250>, meters>)
	UNIT_ADD(length, mil, mils, mil, unit<std::ratio<1000>, feet>)
	UNIT_ADD(length, inch, inches, in, unit<std::ratio<1, 12>, feet>)
	UNIT_ADD(length, mile,   miles,    mi,    unit<std::ratio<5280>, feet>)
	UNIT_ADD(length, nauticalMile, nauticalMiles, nmi, unit<std::ratio<1852>, meters>)
	UNIT_ADD(length, astronicalUnit, astronicalUnits, au, unit<std::ratio<149597870700>, meters>)
	UNIT_ADD(length, lightyear, lightyears, ly, unit<std::ratio<9460730472580800>, meters>)
	UNIT_ADD(length, parsec, parsecs, pc, unit<std::ratio<648000>, astronicalUnits, std::ratio<-1>>)
	UNIT_ADD(length, angstrom, angstroms, angstrom, unit<std::ratio<1, 10>, nanometers>)
	UNIT_ADD(length, cubit, cubits, cbt, unit<std::ratio<18>, inches>)
	UNIT_ADD(length, fathom, fathoms, ftm, unit<std::ratio<6>, feet>)
	UNIT_ADD(length, chain, chains, ch, unit<std::ratio<66>, feet>)
	UNIT_ADD(length, furlong, furlongs, fur, unit<std::ratio<10>, chains>)
	UNIT_ADD(length, hand, hands, hand, unit<std::ratio<4>, inches>)
	UNIT_ADD(length, league, leagues, lea, unit<std::ratio<3>, miles>)
	UNIT_ADD(length, nauticalLeague, nauticalLeagues, nl, unit<std::ratio<3>, nauticalMiles>)
	UNIT_ADD(length, yard, yards, yd, unit<std::ratio<3>, feet>)

	UNIT_ADD_CATEGORY_TRAIT(length)
#endif
