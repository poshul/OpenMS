// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2018.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Lars Nilse $
// $Authors: Lars Nilse, Timo Sachsenberg, Samuel Wein, Mathias Walzer $
// --------------------------------------------------------------------------

#include <OpenMS/config.h>
#include <OpenMS/APPLICATIONS/TOPPBase.h>
#include <OpenMS/CONCEPT/ProgressLogger.h>
#include <OpenMS/DATASTRUCTURES/DBoundingBox.h>
#include <OpenMS/DATASTRUCTURES/ListUtils.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/KERNEL/ConsensusMap.h>
#include <OpenMS/KERNEL/FeatureMap.h>
#include <OpenMS/FORMAT/MzMLFile.h>
#include <OpenMS/FORMAT/FeatureXMLFile.h>
#include <OpenMS/MATH/STATISTICS/LinearRegression.h>
#include <OpenMS/KERNEL/RangeUtils.h>
#include <OpenMS/KERNEL/ChromatogramTools.h>

#include <OpenMS/METADATA/MSQuantifications.h>
#include <OpenMS/TRANSFORMATIONS/RAW2PEAK/PeakPickerHiRes.h>
#include <OpenMS/MATH/STATISTICS/LinearRegression.h>

#include <OpenMS/FORMAT/FeatureXMLFile.h>
#include <OpenMS/FORMAT/ConsensusXMLFile.h>
#include <OpenMS/FORMAT/MzQuantMLFile.h>

#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/MultiplexDeltaMasses.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/MultiplexDeltaMassesGenerator.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/MultiplexIsotopicPeakPattern.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/MultiplexFilteringCentroided.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/MultiplexFilteringProfile.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/MultiplexClustering.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/MultiplexSatelliteCentroided.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/FeatureFinderMultiplexAlgorithm.h>
#include <OpenMS/COMPARISON/CLUSTERING/GridBasedCluster.h>
#include <OpenMS/DATASTRUCTURES/DPosition.h>
#include <OpenMS/DATASTRUCTURES/DBoundingBox.h>

//Contrib includes
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <QDir>

//std includes
#include <cmath>
#include <vector>
#include <algorithm>
#include <fstream>
#include <limits>
#include <locale>
#include <iomanip>

using namespace std;
using namespace OpenMS;
using namespace boost::math;

//-------------------------------------------------------------
//Doxygen docu
//-------------------------------------------------------------

/**
  @page AveragineFilter
  @brief Detects peptide pairs in LC-MS data and determines their relative abundance.
<CENTER>
  <table>
    <tr>
      <td ALIGN = "center" BGCOLOR="#EBEBEB"> pot. predecessor tools </td>
      <td VALIGN="middle" ROWSPAN=3> \f$ \longrightarrow \f$ FeatureFinderMultiplex \f$ \longrightarrow \f$</td>
      <td ALIGN = "center" BGCOLOR="#EBEBEB"> pot. successor tools </td>
    </tr>
    <tr>
      <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> @ref TOPP_FileConverter </td>
      <td VALIGN="middle" ALIGN = "center" ROWSPAN=2> @ref TOPP_IDMapper</td>
    </tr>
    <tr>
      <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> @ref TOPP_FileFilter </td>
    </tr>
  </table>
</CENTER>
  AveragineFilter is a tool to filter precursors to DDA MS2s @image html SILACAnalyzer_algorithm.png
  <B>The command line parameters of this tool are:</B>
  @verbinclude TOPP_FeatureFinderMultiplex.cli
    <B>INI file documentation of this tool:</B>
    @htmlinclude TOPP_FeatureFinderMultiplex.html
*/

// We do not want this class to show up in the docu:
/// @cond TOPPCLASSES

class AveragineFilter :
  public TOPPBase
{
private:

  // input and output files
  String in_;
  String out_;

public:
  AveragineFilter() :
    TOPPBase("AveragineFilter", "Filter DDA data to discard MS2's with parents that don't have the expected isotopic distribution", true)
  {
  }

  void registerOptionsAndFlags_() override
  {
    registerInputFile_("in", "<file>", "", "LC-MS dataset in either centroid or profile mode");
    setValidFormats_("in", ListUtils::create<String>("mzML"));
    registerOutputFile_("out", "<file>", "", "Filtered MS1 and MS2s", false);
    setValidFormats_("out", ListUtils::create<String>("mzML"));

    registerFullParam_(FeatureFinderMultiplexAlgorithm().getDefaults());
  }


  /**
   * @brief process parameters of 'input/output' section
   */
  void getParameters_in_out_()
  {
    in_ = getStringOption_("in");
    out_ = getStringOption_("out");
  }
  
  /**
   * @brief Write peak map to MzML file.
   *
   * @param filename    name of MzML file
   * @param map    Peak map for output
   */
  void writeFilteredMap_(const String& filename, PeakMap& map) const
  {    
    MzMLFile file;
    file.store(filename, map);
  }

  ExitCodes main_(int, const char**) override
  {

    /**
     * handle parameters
     */
    getParameters_in_out_();

    if (out_.empty())
    {
      throw Exception::IllegalArgument(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Strings for all output files are empty. Please specify at least one output file.");
    }


    /* If filter averagine is active
     * PeakMap unfiltered_spectra;
     * options.addMSLevel(1);
     * options.addMSLevel(2);
     * load file
     * PeakMap::ConstIterator last_one;
     * for each spectrum
     *   if mslevel==1
     *     last_one=this_ptr
     *   else if mslevel==2 and precursor mass is known
     *     multiplexFilteringProfile(last_one,....)
     *     if this_ptr->precursormass still in last_one
     *       while this_ptr->precursormass-(1/precursor_charge) exists
     *         this_ptr->precursormass-=1/precursor_charge
     *       copy this_ptr* to spectra
     *
     */

    /**
     * load input
     */
    MzMLFile file;
    MSExperiment exp , filtered_exp;
    
    // only read MS1 and MS2 spectra
    std::vector<int> levels;
    levels.push_back(1);
    levels.push_back(2);
    file.getOptions().setMSLevels(levels);
    
    LOG_DEBUG << "Loading input..." << endl;
    file.setLogType(log_type_);
    file.load(in_, exp);
    bool centroided_;
    double precursorMass;
    PeakMap::ConstIterator last_parent;
    // LOG_DEBUG << "Input file MS1 are centroided: "<< centroided_ << endl;

    for (PeakMap::ConstIterator s_it = exp.begin(); s_it != exp.end();
         ++s_it)
    {
      if (s_it->getMSLevel() == 1 )
      {
        last_parent = s_it;
      }
      if (s_it->getMSLevel() == 2 )
      {
        centroided_ = last_parent->getType(true);
        precursorMass = s_it->getPrecursors()[0].getMZ();

        if (centroided_)
        {
          MultiplexFilteringCentroided filtering(exp_centroid_, patterns, isotopes_per_peptide_min_, isotopes_per_peptide_max_, param_.getValue("algorithm:intensity_cutoff"), param_.getValue("algorithm:rt_band"), param_.getValue("algorithm:mz_tolerance"), (param_.getValue("algorithm:mz_unit") == "ppm"), param_.getValue("algorithm:peptide_similarity"), param_.getValue("algorithm:averagine_similarity"), averagine_similarity_scaling, param_.getValue("algorithm:averagine_type"));
          filtering.setLogType(getLogType());
          std::vector<MultiplexFilteredMSExperiment> filter_results = filtering.filter();
          // DO SOMETHING THERE

        }
        else
        {
          MultiplexFilteringProfile filtering(exp_profile_, exp_centroid_, boundaries_exp_s, patterns, isotopes_per_peptide_min_, isotopes_per_peptide_max_, param_.getValue("algorithm:intensity_cutoff"), param_.getValue("algorithm:rt_band"), param_.getValue("algorithm:mz_tolerance"), (param_.getValue("algorithm:mz_unit") == "ppm"), param_.getValue("algorithm:peptide_similarity"), param_.getValue("algorithm:averagine_similarity"), averagine_similarity_scaling, param_.getValue("algorithm:averagine_type"));
          filtering.setLogType(getLogType());
          std::vector<MultiplexFilteredMSExperiment> filter_results = filtering.filter();
        }
      }
    }

    // write feature map, consensus maps and blacklist
    if (!(out_.empty()))
    {
      writeFilteredMap_(out_, filtered_exp);
    }
    
    return EXECUTION_OK;
  }
  
};

int main(int argc, const char** argv)
{
  AveragineFilter tool;
  return tool.main(argc, argv);
}

//@endcond
