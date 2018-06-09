/*
 * Copyright 2016, Simula Research Laboratory
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <cctag/CCTagMarkersBank.hpp>
#include <cctag/utils/Exceptions.hpp>
#include <cctag/utils/Defines.hpp>

#include <boost/numeric/conversion/bounds.hpp>
#include <boost/throw_exception.hpp>

#include <cmath>
#include <fstream>
#include <vector>
#include <ios>

namespace cctag
{

CCTagMarkersBank::CCTagMarkersBank( std::size_t nCrowns )
{
  _markers.clear();
  if ( nCrowns == 3 )
  {
    for(const auto & idThreeCrown : CCTagMarkersBank::idThreeCrowns)
    {
      std::vector<float> line;
      line.reserve(5);
      for(float j : idThreeCrown)
      {
        line.push_back(j);
      }
      _markers.push_back(line);
    }
    
  }
  else if ( nCrowns == 4 )
  {
    for(const auto & idFourCrown : CCTagMarkersBank::idFourCrowns)
    {
      std::vector<float> line;
      line.reserve(7);
      for(float j : idFourCrown)
      {
        line.push_back(j);
      }
      _markers.push_back(line);
    }
  }
}

CCTagMarkersBank::CCTagMarkersBank( const std::string & file )
{
  read( file );
}


void CCTagMarkersBank::read( const std::string & file )
{
  std::ifstream input( file.c_str() );
  if ( !input.good() )
  {
    //BOOST_THROW_EXCEPTION( exception::Value()
    //                       << exception::dev() + "Unable to open the bank file: " + file );
	throw std::ios_base::failure("Unable to open the bank file: " + file);
  }
  std::string str;
  while ( std::getline( input, str ) )
  {
    std::vector<float> rr;
    cctagLineParse( str.begin(), str.end(), rr);
    if ( rr.size() )
    {
      _markers.push_back( rr );
    }
  }
  input.close();
}

std::size_t CCTagMarkersBank::identify( const std::vector<float> & marker ) const
{
  std::vector< std::vector<float> >::const_iterator itm = _markers.begin();

  std::size_t imin = 0;
  float normMin = boost::numeric::bounds<float>::highest();
  std::size_t i = 0;

  while( itm != _markers.end() )
  {
    std::vector<float>::const_iterator itr1 = marker.begin();
    std::vector<float>::const_iterator itr2 = itm->begin();
    float norm = 0;
    while( itr1 != marker.end() && itr2 != itm->end() )
    {
      norm += ( *itr1 - *itr2 ) * ( *itr1 - *itr2 );
      ++itr1;
      ++itr2;
    }
    ++itm;
    norm = std::sqrt( norm );
    CCTAG_COUT_LILIAN( "Res : " << norm );
    if ( norm < normMin )
    {
      normMin = norm;
      imin = i;
    }
    ++i;
  }

  if ( normMin > 0.6f )
  {
    //BOOST_THROW_EXCEPTION( cctag::exception::Bug() << cctag::exception::dev() + "Unable to identify marker" );
	  throw std::runtime_error("Unable to identify marker");
  }
  else
  {
    return imin + 1;
  }
}

const float CCTagMarkersBank::idThreeCrowns[32][5] =
    {{2.000000f,1.666667f,1.428571f,1.250000f,1.111111f},
    {2.222222f,1.666667f,1.428571f,1.250000f,1.111111f},
    {2.222222f,1.818182f,1.428571f,1.250000f,1.111111f},
    {2.500000f,1.818182f,1.428571f,1.250000f,1.111111f},
    {2.222222f,1.818182f,1.538462f,1.250000f,1.111111f},
    {2.500000f,1.818182f,1.538462f,1.250000f,1.111111f},
    {2.500000f,2.000000f,1.538462f,1.250000f,1.111111f},
    {2.857143f,2.000000f,1.538462f,1.250000f,1.111111f},
    {2.222222f,1.818182f,1.538462f,1.333333f,1.111111f},
    {2.500000f,1.818182f,1.538462f,1.333333f,1.111111f},
    {2.500000f,2.000000f,1.538462f,1.333333f,1.111111f},
    {2.857143f,2.000000f,1.538462f,1.333333f,1.111111f},
    {2.500000f,2.000000f,1.666667f,1.333333f,1.111111f},
    {2.857143f,2.000000f,1.666667f,1.333333f,1.111111f},
    {2.857143f,2.222222f,1.666667f,1.333333f,1.111111f},
    {3.333333f,2.222222f,1.666667f,1.333333f,1.111111f},
    {2.222222f,1.818182f,1.538462f,1.333333f,1.176471f},
    {2.500000f,1.818182f,1.538462f,1.333333f,1.176471f},
    {2.500000f,2.000000f,1.538462f,1.333333f,1.176471f},
    {2.857143f,2.000000f,1.538462f,1.333333f,1.176471f},
    {2.500000f,2.000000f,1.666667f,1.333333f,1.176471f},
    {2.857143f,2.000000f,1.666667f,1.333333f,1.176471f},
    {2.857143f,2.222222f,1.666667f,1.333333f,1.176471f},
    {3.333333f,2.222222f,1.666667f,1.333333f,1.176471f},
    {2.500000f,2.000000f,1.666667f,1.428571f,1.176471f},
    {2.857143f,2.000000f,1.666667f,1.428571f,1.176471f},
    {2.857143f,2.222222f,1.666667f,1.428571f,1.176471f},
    {3.333333f,2.222222f,1.666667f,1.428571f,1.176471f},
    {2.857143f,2.222222f,1.818182f,1.428571f,1.176471f},
    {3.333333f,2.222222f,1.818182f,1.428571f,1.176471f},
    {3.333333f,2.500000f,1.818182f,1.428571f,1.176471f},
    {4.000000f,2.500000f,1.818182f,1.428571f,1.176471f}};

const float CCTagMarkersBank::idFourCrowns[128][7] =
    {2.272727f,1.923077f,1.666667f,1.470588f,1.315789f,1.190476f,1.086957f,
    2.500000f,1.923077f,1.666667f,1.470588f,1.315789f,1.190476f,1.086957f,
    2.500000f,2.083333f,1.666667f,1.470588f,1.315789f,1.190476f,1.086957f,
    2.777778f,2.083333f,1.666667f,1.470588f,1.315789f,1.190476f,1.086957f,
    2.500000f,2.083333f,1.785714f,1.470588f,1.315789f,1.190476f,1.086957f,
    2.777778f,2.083333f,1.785714f,1.470588f,1.315789f,1.190476f,1.086957f,
    2.777778f,2.272727f,1.785714f,1.470588f,1.315789f,1.190476f,1.086957f,
    3.125000f,2.272727f,1.785714f,1.470588f,1.315789f,1.190476f,1.086957f,
    2.500000f,2.083333f,1.785714f,1.562500f,1.315789f,1.190476f,1.086957f,
    2.777778f,2.083333f,1.785714f,1.562500f,1.315789f,1.190476f,1.086957f,
    2.777778f,2.272727f,1.785714f,1.562500f,1.315789f,1.190476f,1.086957f,
    3.125000f,2.272727f,1.785714f,1.562500f,1.315789f,1.190476f,1.086957f,
    2.777778f,2.272727f,1.923077f,1.562500f,1.315789f,1.190476f,1.086957f,
    3.125000f,2.272727f,1.923077f,1.562500f,1.315789f,1.190476f,1.086957f,
    3.125000f,2.500000f,1.923077f,1.562500f,1.315789f,1.190476f,1.086957f,
    3.571429f,2.500000f,1.923077f,1.562500f,1.315789f,1.190476f,1.086957f,
    2.500000f,2.083333f,1.785714f,1.562500f,1.388889f,1.190476f,1.086957f,
    2.777778f,2.083333f,1.785714f,1.562500f,1.388889f,1.190476f,1.086957f,
    2.777778f,2.272727f,1.785714f,1.562500f,1.388889f,1.190476f,1.086957f,
    3.125000f,2.272727f,1.785714f,1.562500f,1.388889f,1.190476f,1.086957f,
    2.777778f,2.272727f,1.923077f,1.562500f,1.388889f,1.190476f,1.086957f,
    3.125000f,2.272727f,1.923077f,1.562500f,1.388889f,1.190476f,1.086957f,
    3.125000f,2.500000f,1.923077f,1.562500f,1.388889f,1.190476f,1.086957f,
    3.571429f,2.500000f,1.923077f,1.562500f,1.388889f,1.190476f,1.086957f,
    2.777778f,2.272727f,1.923077f,1.666667f,1.388889f,1.190476f,1.086957f,
    3.125000f,2.272727f,1.923077f,1.666667f,1.388889f,1.190476f,1.086957f,
    3.125000f,2.500000f,1.923077f,1.666667f,1.388889f,1.190476f,1.086957f,
    3.571429f,2.500000f,1.923077f,1.666667f,1.388889f,1.190476f,1.086957f,
    3.125000f,2.500000f,2.083333f,1.666667f,1.388889f,1.190476f,1.086957f,
    3.571429f,2.500000f,2.083333f,1.666667f,1.388889f,1.190476f,1.086957f,
    3.571429f,2.777778f,2.083333f,1.666667f,1.388889f,1.190476f,1.086957f,
    4.166667f,2.777778f,2.083333f,1.666667f,1.388889f,1.190476f,1.086957f,
    2.500000f,2.083333f,1.785714f,1.562500f,1.388889f,1.250000f,1.086957f,
    2.777778f,2.083333f,1.785714f,1.562500f,1.388889f,1.250000f,1.086957f,
    2.777778f,2.272727f,1.785714f,1.562500f,1.388889f,1.250000f,1.086957f,
    3.125000f,2.272727f,1.785714f,1.562500f,1.388889f,1.250000f,1.086957f,
    2.777778f,2.272727f,1.923077f,1.562500f,1.388889f,1.250000f,1.086957f,
    3.125000f,2.272727f,1.923077f,1.562500f,1.388889f,1.250000f,1.086957f,
    3.125000f,2.500000f,1.923077f,1.562500f,1.388889f,1.250000f,1.086957f,
    3.571429f,2.500000f,1.923077f,1.562500f,1.388889f,1.250000f,1.086957f,
    2.777778f,2.272727f,1.923077f,1.666667f,1.388889f,1.250000f,1.086957f,
    3.125000f,2.272727f,1.923077f,1.666667f,1.388889f,1.250000f,1.086957f,
    3.125000f,2.500000f,1.923077f,1.666667f,1.388889f,1.250000f,1.086957f,
    3.571429f,2.500000f,1.923077f,1.666667f,1.388889f,1.250000f,1.086957f,
    3.125000f,2.500000f,2.083333f,1.666667f,1.388889f,1.250000f,1.086957f,
    3.571429f,2.500000f,2.083333f,1.666667f,1.388889f,1.250000f,1.086957f,
    3.571429f,2.777778f,2.083333f,1.666667f,1.388889f,1.250000f,1.086957f,
    4.166667f,2.777778f,2.083333f,1.666667f,1.388889f,1.250000f,1.086957f,
    2.777778f,2.272727f,1.923077f,1.666667f,1.470588f,1.250000f,1.086957f,
    3.125000f,2.272727f,1.923077f,1.666667f,1.470588f,1.250000f,1.086957f,
    3.125000f,2.500000f,1.923077f,1.666667f,1.470588f,1.250000f,1.086957f,
    3.571429f,2.500000f,1.923077f,1.666667f,1.470588f,1.250000f,1.086957f,
    3.125000f,2.500000f,2.083333f,1.666667f,1.470588f,1.250000f,1.086957f,
    3.571429f,2.500000f,2.083333f,1.666667f,1.470588f,1.250000f,1.086957f,
    3.571429f,2.777778f,2.083333f,1.666667f,1.470588f,1.250000f,1.086957f,
    4.166667f,2.777778f,2.083333f,1.666667f,1.470588f,1.250000f,1.086957f,
    3.125000f,2.500000f,2.083333f,1.785714f,1.470588f,1.250000f,1.086957f,
    3.571429f,2.500000f,2.083333f,1.785714f,1.470588f,1.250000f,1.086957f,
    3.571429f,2.777778f,2.083333f,1.785714f,1.470588f,1.250000f,1.086957f,
    4.166667f,2.777778f,2.083333f,1.785714f,1.470588f,1.250000f,1.086957f,
    3.571429f,2.777778f,2.272727f,1.785714f,1.470588f,1.250000f,1.086957f,
    4.166667f,2.777778f,2.272727f,1.785714f,1.470588f,1.250000f,1.086957f,
    4.166667f,3.125000f,2.272727f,1.785714f,1.470588f,1.250000f,1.086957f,
    5.000000f,3.125000f,2.272727f,1.785714f,1.470588f,1.250000f,1.086957f,
    2.500000f,2.083333f,1.785714f,1.562500f,1.388889f,1.250000f,1.136364f,
    2.777778f,2.083333f,1.785714f,1.562500f,1.388889f,1.250000f,1.136364f,
    2.777778f,2.272727f,1.785714f,1.562500f,1.388889f,1.250000f,1.136364f,
    3.125000f,2.272727f,1.785714f,1.562500f,1.388889f,1.250000f,1.136364f,
    2.777778f,2.272727f,1.923077f,1.562500f,1.388889f,1.250000f,1.136364f,
    3.125000f,2.272727f,1.923077f,1.562500f,1.388889f,1.250000f,1.136364f,
    3.125000f,2.500000f,1.923077f,1.562500f,1.388889f,1.250000f,1.136364f,
    3.571429f,2.500000f,1.923077f,1.562500f,1.388889f,1.250000f,1.136364f,
    2.777778f,2.272727f,1.923077f,1.666667f,1.388889f,1.250000f,1.136364f,
    3.125000f,2.272727f,1.923077f,1.666667f,1.388889f,1.250000f,1.136364f,
    3.125000f,2.500000f,1.923077f,1.666667f,1.388889f,1.250000f,1.136364f,
    3.571429f,2.500000f,1.923077f,1.666667f,1.388889f,1.250000f,1.136364f,
    3.125000f,2.500000f,2.083333f,1.666667f,1.388889f,1.250000f,1.136364f,
    3.571429f,2.500000f,2.083333f,1.666667f,1.388889f,1.250000f,1.136364f,
    3.571429f,2.777778f,2.083333f,1.666667f,1.388889f,1.250000f,1.136364f,
    4.166667f,2.777778f,2.083333f,1.666667f,1.388889f,1.250000f,1.136364f,
    2.777778f,2.272727f,1.923077f,1.666667f,1.470588f,1.250000f,1.136364f,
    3.125000f,2.272727f,1.923077f,1.666667f,1.470588f,1.250000f,1.136364f,
    3.125000f,2.500000f,1.923077f,1.666667f,1.470588f,1.250000f,1.136364f,
    3.571429f,2.500000f,1.923077f,1.666667f,1.470588f,1.250000f,1.136364f,
    3.125000f,2.500000f,2.083333f,1.666667f,1.470588f,1.250000f,1.136364f,
    3.571429f,2.500000f,2.083333f,1.666667f,1.470588f,1.250000f,1.136364f,
    3.571429f,2.777778f,2.083333f,1.666667f,1.470588f,1.250000f,1.136364f,
    4.166667f,2.777778f,2.083333f,1.666667f,1.470588f,1.250000f,1.136364f,
    3.125000f,2.500000f,2.083333f,1.785714f,1.470588f,1.250000f,1.136364f,
    3.571429f,2.500000f,2.083333f,1.785714f,1.470588f,1.250000f,1.136364f,
    3.571429f,2.777778f,2.083333f,1.785714f,1.470588f,1.250000f,1.136364f,
    4.166667f,2.777778f,2.083333f,1.785714f,1.470588f,1.250000f,1.136364f,
    3.571429f,2.777778f,2.272727f,1.785714f,1.470588f,1.250000f,1.136364f,
    4.166667f,2.777778f,2.272727f,1.785714f,1.470588f,1.250000f,1.136364f,
    4.166667f,3.125000f,2.272727f,1.785714f,1.470588f,1.250000f,1.136364f,
    5.000000f,3.125000f,2.272727f,1.785714f,1.470588f,1.250000f,1.136364f,
    2.777778f,2.272727f,1.923077f,1.666667f,1.470588f,1.315789f,1.136364f,
    3.125000f,2.272727f,1.923077f,1.666667f,1.470588f,1.315789f,1.136364f,
    3.125000f,2.500000f,1.923077f,1.666667f,1.470588f,1.315789f,1.136364f,
    3.571429f,2.500000f,1.923077f,1.666667f,1.470588f,1.315789f,1.136364f,
    3.125000f,2.500000f,2.083333f,1.666667f,1.470588f,1.315789f,1.136364f,
    3.571429f,2.500000f,2.083333f,1.666667f,1.470588f,1.315789f,1.136364f,
    3.571429f,2.777778f,2.083333f,1.666667f,1.470588f,1.315789f,1.136364f,
    4.166667f,2.777778f,2.083333f,1.666667f,1.470588f,1.315789f,1.136364f,
    3.125000f,2.500000f,2.083333f,1.785714f,1.470588f,1.315789f,1.136364f,
    3.571429f,2.500000f,2.083333f,1.785714f,1.470588f,1.315789f,1.136364f,
    3.571429f,2.777778f,2.083333f,1.785714f,1.470588f,1.315789f,1.136364f,
    4.166667f,2.777778f,2.083333f,1.785714f,1.470588f,1.315789f,1.136364f,
    3.571429f,2.777778f,2.272727f,1.785714f,1.470588f,1.315789f,1.136364f,
    4.166667f,2.777778f,2.272727f,1.785714f,1.470588f,1.315789f,1.136364f,
    4.166667f,3.125000f,2.272727f,1.785714f,1.470588f,1.315789f,1.136364f,
    5.000000f,3.125000f,2.272727f,1.785714f,1.470588f,1.315789f,1.136364f,
    3.125000f,2.500000f,2.083333f,1.785714f,1.562500f,1.315789f,1.136364f,
    3.571429f,2.500000f,2.083333f,1.785714f,1.562500f,1.315789f,1.136364f,
    3.571429f,2.777778f,2.083333f,1.785714f,1.562500f,1.315789f,1.136364f,
    4.166667f,2.777778f,2.083333f,1.785714f,1.562500f,1.315789f,1.136364f,
    3.571429f,2.777778f,2.272727f,1.785714f,1.562500f,1.315789f,1.136364f,
    4.166667f,2.777778f,2.272727f,1.785714f,1.562500f,1.315789f,1.136364f,
    4.166667f,3.125000f,2.272727f,1.785714f,1.562500f,1.315789f,1.136364f,
    5.000000f,3.125000f,2.272727f,1.785714f,1.562500f,1.315789f,1.136364f,
    3.571429f,2.777778f,2.272727f,1.923077f,1.562500f,1.315789f,1.136364f,
    4.166667f,2.777778f,2.272727f,1.923077f,1.562500f,1.315789f,1.136364f,
    4.166667f,3.125000f,2.272727f,1.923077f,1.562500f,1.315789f,1.136364f,
    5.000000f,3.125000f,2.272727f,1.923077f,1.562500f,1.315789f,1.136364f,
    4.166667f,3.125000f,2.500000f,1.923077f,1.562500f,1.315789f,1.136364f,
    5.000000f,3.125000f,2.500000f,1.923077f,1.562500f,1.315789f,1.136364f,
    5.000000f,3.571429f,2.500000f,1.923077f,1.562500f,1.315789f,1.136364f,
    6.250000f,3.571429f,2.500000f,1.923077f,1.562500f,1.315789f,1.136364f};

} // namespace cctag
