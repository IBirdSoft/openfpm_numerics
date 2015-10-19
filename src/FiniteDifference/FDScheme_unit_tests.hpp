/*
 * FDScheme_unit_tests.hpp
 *
 *  Created on: Oct 5, 2015
 *      Author: i-bird
 */

#ifndef OPENFPM_NUMERICS_SRC_FINITEDIFFERENCE_FDSCHEME_UNIT_TESTS_HPP_
#define OPENFPM_NUMERICS_SRC_FINITEDIFFERENCE_FDSCHEME_UNIT_TESTS_HPP_

#include "FiniteDifference/Derivative.hpp"
#include "FiniteDifference/Laplacian.hpp"

constexpr unsigned int x = 0;
constexpr unsigned int y = 1;
constexpr unsigned int V = 0;

struct sys_nn
{
	// dimensionaly of the equation (2D problem 3D problem ...)
	static const unsigned int dims = 2;
	// number of fields in the system
	static const unsigned int nvar = 1;
	static const unsigned int ord = EQS_FIELD;

	// boundary at X and Y
	static const bool boundary[];

	// type of space float, double, ...
	typedef float stype;
};

const bool sys_nn::boundary[] = {NON_PERIODIC,NON_PERIODIC};

struct sys_pp
{
	// dimensionaly of the equation (2D problem 3D problem ...)
	static const unsigned int dims = 2;
	// number of fields in the system
	static const unsigned int nvar = 1;
	static const unsigned int ord = EQS_FIELD;

	// boundary at X and Y
	static const bool boundary[];

	// type of space float, double, ...
	typedef float stype;
};

const bool sys_pp::boundary[] = {PERIODIC,PERIODIC};

//////////////////////// STAGGERED CASE //////////////////////////////////

struct syss_nn
{
	// dimensionaly of the equation (2D problem 3D problem ...)
	static const unsigned int dims = 2;
	// number of fields in the system
	static const unsigned int nvar = 1;
	static const unsigned int ord = EQS_FIELD;

	static const unsigned int grid = STAGGERED_GRID;

	// boundary at X and Y
	static const bool boundary[];

	// type of space float, double, ...
	typedef float stype;
};

const bool syss_nn::boundary[] = {NON_PERIODIC,NON_PERIODIC};

struct syss_pp
{
	// dimensionaly of the equation (2D problem 3D problem ...)
	static const unsigned int dims = 2;
	// number of fields in the system
	static const unsigned int nvar = 1;
	static const unsigned int ord = EQS_FIELD;

	static const unsigned int grid = STAGGERED_GRID;

	// boundary at X and Y
	static const bool boundary[];

	// type of space float, double, ...
	typedef float stype;
};

const bool syss_pp::boundary[] = {PERIODIC,PERIODIC};

BOOST_AUTO_TEST_SUITE( fd_test )

BOOST_AUTO_TEST_CASE( fd_test_use_non_periodic)
{
	// grid size
	size_t sz[2]={16,16};

	// grid_sm
	grid_sm<2,void> ginfo(sz);

	// Create a derivative row Matrix
	grid_key_dx<2> key11(1,1);
	grid_key_dx<2> key00(0,0);
	grid_key_dx<2> key22(2,2);
	grid_key_dx<2> key1515(15,15);

	// filled colums
	std::unordered_map<long int,float> cols_x;
	std::unordered_map<long int,float> cols_y;

	D<x,Field<V,sys_nn>,sys_nn>::value(key11,ginfo,cols_x,1);
	D<y,Field<V,sys_nn>,sys_nn>::value(key11,ginfo,cols_y,1);

	BOOST_REQUIRE_EQUAL(cols_x.size(),2);
	BOOST_REQUIRE_EQUAL(cols_y.size(),2);

	BOOST_REQUIRE_EQUAL(cols_x[17+1],1);
	BOOST_REQUIRE_EQUAL(cols_x[17-1],-1);

	BOOST_REQUIRE_EQUAL(cols_y[17+16],1);
	BOOST_REQUIRE_EQUAL(cols_y[17-16],-1);

	// filled colums

	std::unordered_map<long int,float> cols_xx;
	std::unordered_map<long int,float> cols_xy;
	std::unordered_map<long int,float> cols_yx;
	std::unordered_map<long int,float> cols_yy;

	// Composed derivative

	D<x,D<x,Field<V,sys_nn>,sys_nn>,sys_nn>::value(key22,ginfo,cols_xx,1);
	D<x,D<y,Field<V,sys_nn>,sys_nn>,sys_nn>::value(key22,ginfo,cols_xy,1);
	D<y,D<x,Field<V,sys_nn>,sys_nn>,sys_nn>::value(key22,ginfo,cols_yx,1);
	D<y,D<y,Field<V,sys_nn>,sys_nn>,sys_nn>::value(key22,ginfo,cols_yy,1);

	BOOST_REQUIRE_EQUAL(cols_xx.size(),3);
	BOOST_REQUIRE_EQUAL(cols_xy.size(),4);
	BOOST_REQUIRE_EQUAL(cols_yx.size(),4);
	BOOST_REQUIRE_EQUAL(cols_yy.size(),3);

	BOOST_REQUIRE_EQUAL(cols_xx[32],1);
	BOOST_REQUIRE_EQUAL(cols_xx[34],-2);
	BOOST_REQUIRE_EQUAL(cols_xx[36],1);

	BOOST_REQUIRE_EQUAL(cols_xy[17],1);
	BOOST_REQUIRE_EQUAL(cols_xy[19],-1);
	BOOST_REQUIRE_EQUAL(cols_xy[49],-1);
	BOOST_REQUIRE_EQUAL(cols_xy[51],1);

	BOOST_REQUIRE_EQUAL(cols_yx[17],1);
	BOOST_REQUIRE_EQUAL(cols_yx[19],-1);
	BOOST_REQUIRE_EQUAL(cols_yx[49],-1);
	BOOST_REQUIRE_EQUAL(cols_xy[51],1);

	BOOST_REQUIRE_EQUAL(cols_yy[2],1);
	BOOST_REQUIRE_EQUAL(cols_yy[34],-2);
	BOOST_REQUIRE_EQUAL(cols_yy[66],1);

	// Non periodic with one sided

	cols_x.clear();
	cols_y.clear();

	D<x,Field<V,sys_nn>,sys_nn,CENTRAL_B_ONE_SIDE>::value(key11,ginfo,cols_x,1);
	D<y,Field<V,sys_nn>,sys_nn,CENTRAL_B_ONE_SIDE>::value(key11,ginfo,cols_y,1);

	BOOST_REQUIRE_EQUAL(cols_x.size(),2);
	BOOST_REQUIRE_EQUAL(cols_y.size(),2);

	BOOST_REQUIRE_EQUAL(cols_x[17+1],1);
	BOOST_REQUIRE_EQUAL(cols_x[17-1],-1);

	BOOST_REQUIRE_EQUAL(cols_y[17+16],1);
	BOOST_REQUIRE_EQUAL(cols_y[17-16],-1);

	// Border left

	cols_x.clear();
	cols_y.clear();

	D<x,Field<V,sys_nn>,sys_nn,CENTRAL_B_ONE_SIDE>::value(key00,ginfo,cols_x,1);
	D<y,Field<V,sys_nn>,sys_nn,CENTRAL_B_ONE_SIDE>::value(key00,ginfo,cols_y,1);

	BOOST_REQUIRE_EQUAL(cols_x.size(),3);
	BOOST_REQUIRE_EQUAL(cols_y.size(),3);

	BOOST_REQUIRE_EQUAL(cols_x[0],-1.5);
	BOOST_REQUIRE_EQUAL(cols_x[1],2);
	BOOST_REQUIRE_EQUAL(cols_x[2],-0.5);

	BOOST_REQUIRE_EQUAL(cols_y[0],-1.5);
	BOOST_REQUIRE_EQUAL(cols_y[16],2);
	BOOST_REQUIRE_EQUAL(cols_y[32],-0.5);

	// Border Top right

	cols_x.clear();
	cols_y.clear();

	D<x,Field<V,sys_nn>,sys_nn,CENTRAL_B_ONE_SIDE>::value(key1515,ginfo,cols_x,1);
	D<y,Field<V,sys_nn>,sys_nn,CENTRAL_B_ONE_SIDE>::value(key1515,ginfo,cols_y,1);

	BOOST_REQUIRE_EQUAL(cols_x.size(),3);
	BOOST_REQUIRE_EQUAL(cols_y.size(),3);

	BOOST_REQUIRE_EQUAL(cols_x[15*16+15],1.5);
	BOOST_REQUIRE_EQUAL(cols_x[15*16+14],-2);
	BOOST_REQUIRE_EQUAL(cols_x[15*16+13],0.5);

	BOOST_REQUIRE_EQUAL(cols_y[15*16+15],1.5);
	BOOST_REQUIRE_EQUAL(cols_y[14*16+15],-2);
	BOOST_REQUIRE_EQUAL(cols_y[13*16+15],0.5);
}

BOOST_AUTO_TEST_CASE( fd_test_use_periodic)
{
	// grid size
	size_t sz[2]={16,16};

	// grid_sm
	grid_sm<2,void> ginfo(sz);

	// Create a derivative row Matrix
	grid_key_dx<2> key11(1,1);
	grid_key_dx<2> key00(0,0);
	grid_key_dx<2> key22(2,2);
	grid_key_dx<2> key1515(15,15);

	// filled colums
	std::unordered_map<long int,float> cols_x;
	std::unordered_map<long int,float> cols_y;

	D<x,Field<V,sys_pp>,sys_pp>::value(key11,ginfo,cols_x,1);
	D<y,Field<V,sys_pp>,sys_pp>::value(key11,ginfo,cols_y,1);

	BOOST_REQUIRE_EQUAL(cols_x.size(),2);
	BOOST_REQUIRE_EQUAL(cols_y.size(),2);

	BOOST_REQUIRE_EQUAL(cols_x[17+1],1);
	BOOST_REQUIRE_EQUAL(cols_x[17-1],-1);

	BOOST_REQUIRE_EQUAL(cols_y[17+16],1);
	BOOST_REQUIRE_EQUAL(cols_y[17-16],-1);

	cols_x.clear();
	cols_y.clear();

	D<x,Field<V,sys_pp>,sys_pp>::value(key00,ginfo,cols_x,1);
	D<y,Field<V,sys_pp>,sys_pp>::value(key00,ginfo,cols_y,1);

	BOOST_REQUIRE_EQUAL(cols_x.size(),2);
	BOOST_REQUIRE_EQUAL(cols_y.size(),2);

	BOOST_REQUIRE_EQUAL(cols_x[1],1);
	BOOST_REQUIRE_EQUAL(cols_x[15],-1);

	BOOST_REQUIRE_EQUAL(cols_y[16],1);
	BOOST_REQUIRE_EQUAL(cols_y[16*15],-1);

	// periodic composed derivative

	std::unordered_map<long int,float> cols_xx;
	std::unordered_map<long int,float> cols_xy;
	std::unordered_map<long int,float> cols_yx;
	std::unordered_map<long int,float> cols_yy;

	D<x,D<x,Field<V,sys_pp>,sys_pp>,sys_pp>::value(key00,ginfo,cols_xx,1);
	D<x,D<y,Field<V,sys_pp>,sys_pp>,sys_pp>::value(key00,ginfo,cols_xy,1);
	D<y,D<x,Field<V,sys_pp>,sys_pp>,sys_pp>::value(key00,ginfo,cols_yx,1);
	D<y,D<y,Field<V,sys_pp>,sys_pp>,sys_pp>::value(key00,ginfo,cols_yy,1);

	BOOST_REQUIRE_EQUAL(cols_xx.size(),3);
	BOOST_REQUIRE_EQUAL(cols_xy.size(),4);
	BOOST_REQUIRE_EQUAL(cols_yx.size(),4);
	BOOST_REQUIRE_EQUAL(cols_yy.size(),3);

	BOOST_REQUIRE_EQUAL(cols_xx[14],1);
	BOOST_REQUIRE_EQUAL(cols_xx[0],-2);
	BOOST_REQUIRE_EQUAL(cols_xx[2],1);

	BOOST_REQUIRE_EQUAL(cols_xy[17],1);
	BOOST_REQUIRE_EQUAL(cols_xy[16+15],-1);
	BOOST_REQUIRE_EQUAL(cols_xy[15*16+1],-1);
	BOOST_REQUIRE_EQUAL(cols_xy[15*16+15],1);

	BOOST_REQUIRE_EQUAL(cols_yx[17],1);
	BOOST_REQUIRE_EQUAL(cols_yx[16+15],-1);
	BOOST_REQUIRE_EQUAL(cols_yx[15*16+1],-1);
	BOOST_REQUIRE_EQUAL(cols_xy[15*16+15],1);

	BOOST_REQUIRE_EQUAL(cols_yy[32],1);
	BOOST_REQUIRE_EQUAL(cols_yy[0],-2);
	BOOST_REQUIRE_EQUAL(cols_yy[14*16],1);

	// Border Top right

	cols_x.clear();
	cols_y.clear();

	D<x,Field<V,sys_pp>,sys_pp>::value(key1515,ginfo,cols_x,1);
	D<y,Field<V,sys_pp>,sys_pp>::value(key1515,ginfo,cols_y,1);

	BOOST_REQUIRE_EQUAL(cols_x.size(),2);
	BOOST_REQUIRE_EQUAL(cols_y.size(),2);

	BOOST_REQUIRE_EQUAL(cols_x[15*16+0],1);
	BOOST_REQUIRE_EQUAL(cols_x[15*16+14],-1);

	BOOST_REQUIRE_EQUAL(cols_y[0*16+15],1);
	BOOST_REQUIRE_EQUAL(cols_y[14*16+15],-1);
}

/////////////// Laplacian test

BOOST_AUTO_TEST_CASE( fd_test_lap_use_periodic)
{
	// grid size
	size_t sz[2]={16,16};

	// grid_sm
	grid_sm<2,void> ginfo(sz);

	// Create a derivative row Matrix
	grid_key_dx<2> key11(1,1);
	grid_key_dx<2> key00(0,0);
	grid_key_dx<2> key22(2,2);
	grid_key_dx<2> key1515(15,15);

	// filled colums
	std::unordered_map<long int,float> cols;

	Lap<Field<V,sys_pp>,sys_pp>::value(key11,ginfo,cols,1);

	BOOST_REQUIRE_EQUAL(cols.size(),5);

	BOOST_REQUIRE_EQUAL(cols[1],1);
	BOOST_REQUIRE_EQUAL(cols[17-1],1);
	BOOST_REQUIRE_EQUAL(cols[17+1],1);
	BOOST_REQUIRE_EQUAL(cols[17+16],1);

	BOOST_REQUIRE_EQUAL(cols[17],-4);

	cols.clear();

	Lap<Field<V,sys_pp>,sys_pp>::value(key00,ginfo,cols,1);

	BOOST_REQUIRE_EQUAL(cols.size(),5);

	BOOST_REQUIRE_EQUAL(cols[1],1);
	BOOST_REQUIRE_EQUAL(cols[15],1);
	BOOST_REQUIRE_EQUAL(cols[16],1);
	BOOST_REQUIRE_EQUAL(cols[15*16],1);

	BOOST_REQUIRE_EQUAL(cols[0],-4);

	// Border Top right

	cols.clear();

	Lap<Field<V,sys_pp>,sys_pp>::value(key1515,ginfo,cols,1);

	BOOST_REQUIRE_EQUAL(cols.size(),5);

	BOOST_REQUIRE_EQUAL(cols[15*16+14],1);
	BOOST_REQUIRE_EQUAL(cols[15*16],1);
	BOOST_REQUIRE_EQUAL(cols[14*16+15],1);
	BOOST_REQUIRE_EQUAL(cols[15],1);

	BOOST_REQUIRE_EQUAL(cols[15*16+15],-4);
}

//////////////// Position ////////////////////

BOOST_AUTO_TEST_CASE( fd_test_use_staggered_position)
{
	// grid size
/*	size_t sz[2]={16,16};

	// Create a derivative row Matrix
	grid_key_dx<2> key11(1,1);
	grid_key_dx<2> key00(0,0);
	grid_key_dx<2> key22(2,2);
	grid_key_dx<2> key1515(15,15);

	openfpm::vector<comb<2>> vx_c;
	comb<2> cx = {{1,0}};
	vx_c.add(cx);         // like v_x

	openfpm::vector<comb<2>> vy_c;
	comb<2> cy = {{0,1}};
	vy_c.add(cy);         // like v_y

	grid_key_dx<2> key_ret_x = D<x,Field<V,sys_nn>,sys_nn>::position(key11,0,vx_c);
	grid_key_dx<2> key_ret_y = D<y,Field<V,sys_nn>,sys_nn>::position(key11,0,vx_c);

	BOOST_REQUIRE_EQUAL(key_ret_x.get(0),1);
	BOOST_REQUIRE_EQUAL(key_ret_x.get(1),1);

	BOOST_REQUIRE_EQUAL(key_ret_y.get(0),0);
	BOOST_REQUIRE_EQUAL(key_ret_y.get(1),0);

	key_ret_x = D<x,Field<V,sys_nn>,sys_nn>::position(key11,0,vy_c);
	key_ret_y = D<y,Field<V,sys_nn>,sys_nn>::position(key11,0,vy_c);

	BOOST_REQUIRE_EQUAL(key_ret_x.get(0),0);
	BOOST_REQUIRE_EQUAL(key_ret_x.get(1),0);

	BOOST_REQUIRE_EQUAL(key_ret_y.get(0),1);
	BOOST_REQUIRE_EQUAL(key_ret_y.get(1),1);

	// Composed derivative

	grid_key_dx<2> key_ret_xx = D<x,D<x,Field<V,sys_nn>,sys_nn>,sys_nn>::position(key00,0);
	grid_key_dx<2> key_ret_xy = D<x,D<y,Field<V,sys_nn>,sys_nn>,sys_nn>::position(key00,0);
	grid_key_dx<2> key_ret_yx = D<y,D<x,Field<V,sys_nn>,sys_nn>,sys_nn>::position(key00,0);
	grid_key_dx<2> key_ret_yy = D<y,D<y,Field<V,sys_nn>,sys_nn>,sys_nn>::position(key00,0);

	BOOST_REQUIRE_EQUAL(key_ret_xx.get(0),1);
	BOOST_REQUIRE_EQUAL(key_ret_xx.get(1),0);

	BOOST_REQUIRE_EQUAL(key_ret_xy.get(0),1);
	BOOST_REQUIRE_EQUAL(key_ret_xy.get(1),0);

	BOOST_REQUIRE_EQUAL(key_ret_xy.get(0),0);
	BOOST_REQUIRE_EQUAL(key_ret_xy.get(1),1);

	BOOST_REQUIRE_EQUAL(key_ret_xy.get(0),0);
	BOOST_REQUIRE_EQUAL(key_ret_xy.get(1),1);

	////////////////// Non periodic with one sided

	key_ret_x = D<x,Field<V,sys_nn>,sys_nn,CENTRAL_B_ONE_SIDE>::position(key00,0,vx_c);
	key_ret_y = D<y,Field<V,sys_nn>,sys_nn,CENTRAL_B_ONE_SIDE>::position(key00,0,vx_c);

	BOOST_REQUIRE_EQUAL(key_ret_x.get(0),0);
	BOOST_REQUIRE_EQUAL(key_ret_x.get(1),1);

	BOOST_REQUIRE_EQUAL(key_ret_x,0);
	BOOST_REQUIRE_EQUAL(key_ret_y,0);

	// Border left*/


}

BOOST_AUTO_TEST_SUITE_END()


#endif /* OPENFPM_NUMERICS_SRC_FINITEDIFFERENCE_FDSCHEME_UNIT_TESTS_HPP_ */