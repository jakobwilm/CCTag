/*
 * Copyright 2016, Simula Research Laboratory
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
//#include <getopt.h>
#include <iostream>
#include <string>
#include <memory>


#include "CmdLine.hpp"

namespace cctag {

//#define no_argument       0 
//#define required_argument 1 
//#define optional_argument 2
//
//static const struct option longopts[] =
//{
//    {"input",      required_argument, 0, 'i'},
//    {"nbrings",    required_argument, 0, 'n'},    
//    {"bank",       required_argument, 0, 'b'},
//    {"parameters", required_argument, 0, 'p'},
//    {"output",     optional_argument, 0, 'o'},   
//#ifdef WITH_CUDA
//    {"sync",       no_argument,       0, 0xd0 },
//    {"debug-dir",  required_argument, 0, 0xd1 },
//    {"use-cuda",   no_argument,       0, 0xd2 },
//    {"parallel",   required_argument, 0, 0xd3 },
//#endif
//    {0,0,0,0},
//};

CmdLine::CmdLine( )
    : _filename( "" )
    , _nRings( 0 )
    , _cctagBankFilename( "" )
    , _paramsFilename( "" )
    , _outputFolderName( "" )
#ifdef WITH_CUDA
    , _switchSync( false )
    , _debugDir( "" )
    , _useCuda( false )
    , _parallel( 1 )
#endif
	, _allParams("Program for detecting CCTags in images or in a video")
{ 
	using namespace boost::program_options;

	options_description required("Required input parameters");
	required.add_options()
		("input,i", value<std::string>(&_filename)->required(), "Path to an image (JPG, PNG) or video (avi, mov) or camera index for live capture (0, 1...)")
		("nbrings,n", value<std::size_t>(&_nRings)->required(), "Number of rings of the CCTags to detect");

	options_description optional("Optional parameters");
	optional.add_options()
		("bank,b", value<std::string>(&_cctagBankFilename)->default_value(_cctagBankFilename), "Path to a bank parameter file, e.g. 4Crowns/ids.txt")
		("params,p", value<std::string>(&_paramsFilename)->default_value(_paramsFilename), "Path to configuration XML file")
		("output,o", value<std::string>(&_outputFolderName)->default_value(_outputFolderName), "Output folder name")
#ifdef WITH_CUDA
		("sync", bool_switch(&_switchSync), "CUDA debug option, run all CUDA ops synchronously")
		("use-cuda", bool_switch(&_useCuda), "Select GPU code instead of CPU code")
		("debug-dir", value<std::string>(&_debugDir)->default_value(_debugDir), "Path storing image to debug intermediate GPU results")
		("parallel", value<int>(&_parallel)->default_value(_parallel), "Use <n> CUDA pipes concurrently (default 1)")
#endif
		;

	_allParams.add(required).add(optional);
}

bool CmdLine::parse( int argc, char* argv[] )
{
//  int index;
//  int iarg=0;
//
//  // bools to check that mandatory parameters are present
//  bool has_i = false;
//  bool has_n = false;
//
//  //turn off getopt error message
//  // opterr=1; 
//
//  while(iarg != -1)
//  {
//    iarg = getopt_long(argc, argv, "i:n:b:p:o:", longopts, &index);
//
//    switch (iarg)
//    {
//      case 'i'  : _filename          = optarg; has_i = true; break;
//      case 'n'  : _nCrowns           = optarg; has_n = true; break;
//      case 'b'  : _cctagBankFilename = optarg; break;
//      case 'p'  : _paramsFilename    = optarg; break;
//      case 'o'  : _outputFolderName  = optarg; break;
//#ifdef WITH_CUDA
//      case 0xd0 : _switchSync        = true;   break;
//      case 0xd1 : _debugDir          = optarg; break;
//      case 0xd2 : _useCuda           = true;   break;
//      case 0xd3 : _parallel          = strtol( optarg, NULL, 0 );   break;
//#endif
//      default : break;
//    }
//  }
//  return ( has_i && has_n );

	using namespace boost::program_options;

	variables_map vm;
	try
	{
		store(parse_command_line(argc, argv, _allParams), vm);
		if (vm.count("help") || (argc == 1))
		{
			return false;
		}
		notify(vm);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
}

void CmdLine::print( const char* const argv0 )
{
    std::cout << "You called " << argv0 << " with:" << std::endl
         << "    --input     " << _filename << std::endl
         << "    --nbrings     " << _nRings << std::endl
         << "    --bank      " << _cctagBankFilename << std::endl
         << "    --params    " << _paramsFilename << std::endl
         << "    --output    " << _outputFolderName << std::endl;
#ifdef WITH_CUDA
	std::cout << "    --parallel " << _parallel << std::endl;
    if( _switchSync )
        std::cout << "    --sync " << std::endl;
    if( _debugDir != "" )
        std::cout << "    --debug-dir " << _debugDir << std::endl;
    if( _useCuda )
        std::cout << "    --use-cuda " << std::endl;
#endif
    std::cout << std::endl;
}

void CmdLine::usage( const char* const argv0 )
{
  //std::cerr << "Usage: " << argv0 << "<parameters>\n"
  //        "    Mandatory:\n"
  //        "           [-i|--input] <imgpath>\n"
  //        "           [-n|--nbrings] <nbrings>\n"
  //        "    Optional:\n"
  //        "           [-p|--params <confpath>]\n"
  //        "           [-b|--bank] <bankpath>\n"
  //        "           [-o|--output] <outputfoldername>\n"
  //        "           [--sync]\n"
  //        "           [--debug-dir <debugdir>]\n"
  //        "           [--use-cuda]\n"
  //        "           [--parallel <n>]\n"
  //        "\n"
  //        "    <imgpath>  - path to an image (JPG, PNG) or video (avi, mov) or camera index for live capture (0, 1...)\n"
  //        "    <nbrings>  - number of rings of the CCTags to detect\n"
  //        "    <bankpath> - path to a bank parameter file, e.g. 4Crowns/ids.txt \n"
  //        "    <output>   - output folder name \n"
  //        "    <confpath> - path to configuration XML file \n"
  //        "    --sync     - CUDA debug option, run all CUDA ops synchronously\n"
  //        "    <debugdir> - path storing image to debug intermediate GPU results\n"
  //        "    --use-cuda - select GPU code instead of CPU code\n"
  //        "    --parallel - use <n> CUDA pipes concurrently (default 1)\n"
  //        "\n" << std::endl;
	std::cout << _allParams << std::endl;
}

}

