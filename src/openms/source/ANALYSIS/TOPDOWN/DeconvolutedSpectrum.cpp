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
// $Maintainer: Kyowon Jeong, Jihyung Kim $
// $Authors: Kyowon Jeong, Jihyung Kim $
// --------------------------------------------------------------------------
//
// Created by Kyowon Jeong on 4/22/20.
//

#include "include/OpenMS/ANALYSIS/TOPDOWN/DeconvolutedSpectrum.h"

namespace OpenMS
{
  DeconvolutedSpectrum::DeconvolutedSpectrum()
  {
  }

  DeconvolutedSpectrum::DeconvolutedSpectrum(MSSpectrum &s, int &n) :
      spec(&s), scanNumber(n)
  {
  }

  DeconvolutedSpectrum::~DeconvolutedSpectrum()
  {
    std::vector<LogMzPeak>().swap(peaks);
    std::vector<PeakGroup>().swap(peakGroups);
    //std::unordered_map<int, PeakGroup>().swap(peakGroupMap);
  }

  bool DeconvolutedSpectrum::empty() const
  {
    return peakGroups.empty();
  }

  void DeconvolutedSpectrum::clearChargeSNRMap(){
    for(auto &pg : peakGroups){
      pg.clearChargeSNRMap();
    }
  }
  /*void DeconvolutedSpectrum::updatePeakGroupMap()
  {
    if (!peakGroupMap.empty())
    {
      return;
    }
    //std::cout<< peaks.size()<<std::endl;

    for (auto &pg : peakGroups)
    {
      for (auto &p : pg.peaks)
      {
        auto position = p.index;
        if (peakGroupMap.find(position) == peakGroupMap.end())
        {
          peakGroupMap[position] = pg;
        }
        else
        {
          if (peakGroupMap[position].isotopeCosineScore < pg.isotopeCosineScore)
          {
            peakGroupMap[position] = pg;
          }
          continue;
        }
      }
    }
    //std::cout<<"2"<<std::endl;
  }*/


  void DeconvolutedSpectrum::writeDeconvolutedMasses(std::fstream &fs,
                                                     FLASHDeconvHelperStructs::Parameter &param)//, fstream &fsm, fstream &fsp)
  {
    if (empty())
    {
      return;
    }

    for (auto &pg : peakGroups)
    {
      if (pg.peaks.empty())
      {
        continue;
      }
      double &m = pg.monoisotopicMass;
      double &am = pg.avgMass;
      double &intensity = pg.intensity;
      int minCharge = param.chargeRange + param.minCharge;
      int maxCharge = -1;
      for (auto &p : pg.peaks)
      {
        minCharge = minCharge < p.charge ? minCharge : p.charge;
        maxCharge = maxCharge > p.charge ? maxCharge : p.charge;
      }

      fs << pg.massIndex << "\t" << specIndex << "\t" << param.fileName << "\t" << spec->getNativeID() << "\t"
         << spec->getMSLevel() << "\t"
         << massCntr << "\t"
         << std::to_string(am) << "\t" << std::to_string(m) << "\t" << intensity << "\t"
         << (maxCharge - minCharge + 1) << "\t" << minCharge << "\t" << maxCharge << "\t"
         << std::to_string(spec->getRT())
         << "\t" << pg.peaks.size() << "\t";
      fs << std::fixed << std::setprecision(2);
      fs << pg.maxQScoreCharge << "\t" << pg.maxQScoreMzStart << "\t" << pg.maxQScoreMzEnd << "\t";
      fs << std::fixed << std::setprecision(-1);
      if (spec->getMSLevel() > 1)
      {
        if (precursorPeakGroup == nullptr){
          fs << "N/A\tN/A\tN/A\tN/A\tN/A\tN/A\tN/A\tN/A\tN/A\tN/A\t";
        }else{
          fs << precursorPeakGroup->specIndex << "\t" << precursorPeak->mz << "\t" << precursorPeak->charge
              << "\t" << precursorPeakGroup->perChargeSNR[precursorPeak->charge]  << "\t" << precursorPeak->intensity
              << "\t" << precursorPeakGroup->monoisotopicMass << "\t" << precursorPeakGroup->totalSNR << "\t" << precursorPeakGroup->isotopeCosineScore
              << "\t" << precursorPeakGroup->chargeCosineScore << "\t" <<precursorPeakGroup->intensity << "\t";
        }
      }

      if (param.writeDetail)
      {
        fs << std::fixed << std::setprecision(2);
        for (auto &p : pg.peaks)
        {
          fs << p.mz << ";";
        }
        fs << "\t";
        for (auto &p : pg.peaks)
        {
          fs << p.charge << ";";
        }
        fs << "\t";
        for (auto &p : pg.peaks)
        {
          fs << p.getUnchargedMass() << ";";
        }
        fs << "\t";
        for (auto &p : pg.peaks)
        {
          fs << p.isotopeIndex << ";";
        }
        fs << "\t";

        for (auto &p : pg.peaks)
        {
          auto tm = pg.monoisotopicMass + p.isotopeIndex * Constants::ISOTOPE_MASSDIFF_55K_U;
          auto diff = (tm / p.charge + Constants::PROTON_MASS_U - p.mz) / p.mz;

          fs << 1e6 * diff << ";";
        }
        fs << "\t";

        fs << std::fixed << std::setprecision(1);
        for (auto &p : pg.peaks)
        {
          fs << p.intensity << ";";
        }
        fs << "\t";
      }

      fs << std::fixed << std::setprecision(3);
      fs << pg.isotopeCosineScore;

      if (spec->getMSLevel() == 1)
      {
        fs << "\t" << pg.chargeCosineScore;
      }

      fs << "\t" << pg.totalSNR << "\t" << pg.qScore << "\n" << std::setprecision(-1); //
    }

  }


  void DeconvolutedSpectrum::writeDeconvolutedMassesHeader(std::fstream &fs, int &n, bool detail)
  {
    if (detail)
    {
      if (n == 1)
      {
        fs
            << "MassIndex\tSpecIndex\tFileName\tSpecID\tMSLevel\tMassCountInSpec\tAvgMass\tMonoisotopicMass\t"
               "AggregatedIntensity\tPeakChargeRange\tPeakMinCharge\tPeakMaxCharge\t"
               "RetentionTime\tPeakCount\tMaxQScoreCharge\tMaxQScoreMzStart\tMaxQScoreMzEnd\t"
               "PeakMZs\tPeakCharges\tPeakMasses\tPeakIsotopeIndices\tPeakPPMErrors\tPeakIntensities\t"
               "IsotopeCosine\tChargeIntensityCosine\tMassSNR\tMaxQScore\n";
      }
      else
      {
        fs
            << "MassIndex\tSpecIndex\tFileName\tSpecID\tMSLevel\tMassCountInSpec\tAvgMass\tMonoisotopicMass\t"
               "AggregatedIntensity\tPeakChargeRange\tPeakMinCharge\tPeakMaxCharge\t"
               "RetentionTime\tPeakCount\tMaxQScoreCharge\tMaxQScoreMzStart\tMaxQScoreMzEnd\t"
               "PrecursorSpecIndex\tPrecursorMz\tPrecursorCharge\tPrecursorChargeSNR\tPrecursorIntensity\t"
               "PrecursorMonoMass\tPrecursorMassSNR\tPrecursorIsotopeCosine\tPrecursorChargeIntensityCosine\tPrecursorMassIntensity\t"
               "PeakMZs\tPeakCharges\tPeakMasses\tPeakIsotopeIndices\tPeakPPMErrors\tPeakIntensities\t"
               "IsotopeCosine\tMassSNR\tMaxQScore\n";
      }
    }
    else
    {
      if (n == 1)
      {
        fs
            << "MassIndex\tSpecIndex\tFileName\tSpecID\tMSLevel\tMassCountInSpec\tAvgMass\tMonoisotopicMass\t"
               "AggregatedIntensity\tPeakChargeRange\tPeakMinCharge\tPeakMaxCharge\t"
               "RetentionTime\tPeakCount\tMaxQScoreCharge\tMaxQScoreMzStart\tMaxQScoreMzEnd\t"
               //"PeakMZs\tPeakCharges\tPeakMasses\tPeakIsotopeIndices\tPeakPPMErrors\tPeakIntensities\t"
               "IsotopeCosine\tChargeIntensityCosine\tMassSNR\tMaxQScore\n";
      }
      else
      {
        fs
            <<  "MassIndex\tSpecIndex\tFileName\tSpecID\tMSLevel\tMassCountInSpec\tAvgMass\tMonoisotopicMass\t"
                "AggregatedIntensity\tPeakChargeRange\tPeakMinCharge\tPeakMaxCharge\t"
                "RetentionTime\tPeakCount\tMaxQScoreCharge\tMaxQScoreMzStart\tMaxQScoreMzEnd\t"
                "PrecursorSpecIndex\tPrecursorMz\tPrecursorCharge\tPrecursorChargeSNR\tPrecursorIntensity\t"
                "PrecursorMonoMass\tPrecursorMassSNR\tPrecursorIsotopeCosine\tPrecursorChargeIntensityCosine\tPrecursorMassIntensity\t"
             //   "PeakMZs\tPeakCharges\tPeakMasses\tPeakIsotopeIndices\tPeakPPMErrors\tPeakIntensities\t"
                "IsotopeCosine\tMassSNR\tMaxQScore\n";
      }

    }
    //pg.maxSNRcharge << "\t" << pg.maxSNR << "\t" << pg.maxSNRminMz << "\t" << pg.maxSNRmaxMz
    //MinScan	MaxScan	 RepScan	RepCharge	RepMz ApexScanNum    Envelope

  }

  void DeconvolutedSpectrum::writeAttCsvHeader(std::fstream &fs){
    fs<<"ScanNumber,PrecursorScanNumber,Charge,ChargeSNR,PeakIntensity,PeakMz,"
        "MonoMass,MassSNR,IsotopeCosine,ChargeIntensityCosine,MassIntensity,QScore,Class\n";
  }

  void DeconvolutedSpectrum::writeAttCsv(std::fstream &fs, int msLevel, double qScoreThreshold = -1000){

    if (msLevel>1)
    {
      if(precursorPeakGroup != nullptr)
      {
        fs << scanNumber << "," << precursorPeakGroup->scanNumber << "," << precursorPeak->charge
           << "," << log10(precursorPeakGroup->perChargeSNR[precursorPeak->charge] + 1e-3) << ","
           << log10(precursorPeak->intensity + 1)
            //<< "," << log10(originalPrecursorPeak->getIntensity() + 1)
            << "," << precursorPeak->mz
           << "," << precursorPeakGroup->monoisotopicMass << "," << log10(precursorPeakGroup->totalSNR + 1e-3) << ","
           << precursorPeakGroup->isotopeCosineScore
           << "," << precursorPeakGroup->chargeCosineScore << "," << log10(precursorPeakGroup->intensity + 1)
           << "," << getQScore()
           << ",f\n";
      }else{
          fs << scanNumber << "," << 0 << "," << 0
             << "," << log10(1e-3) << ","
             << (precursorPeak == nullptr? log10(1) : log10(precursorPeak->intensity + 1))
              //<< "," << (originalPrecursorPeak == nullptr? log10(1) : log10(originalPrecursorPeak->getIntensity() + 1))
              << "," << (precursorPeak == nullptr? (0) : (precursorPeak->mz))
             << "," << 0 << "," << log10(1e-3) << ","
             << 0
             << "," << 0 << "," << log10(1)
             << "," << getQScore()
             << ",f\n";
      }
    }else{
      //for(auto & p : peaks){
        //std::cout<<std::to_string(p.mz) << " " << std::to_string(p.intensity) <<std::endl;
      //}
     // int numMS2 = 1; // take as an input..


      int size = 0;
      for (auto &pg : peakGroups)
      {
        if(pg.qScore < qScoreThreshold){
          continue;
        }
        LogMzPeak *peak = nullptr;
        double maxIntensity = 0;
        for (auto &p : pg.peaks)
        {
          if (p.charge != pg.maxQScoreCharge){
            continue;
          }

          if (p.mz < pg.maxQScoreMzStart){
            continue;
          }
          if (p.mz > pg.maxQScoreMzEnd){
            break;
          }

          if(p.intensity < maxIntensity){
            continue;
          }
          maxIntensity = p.intensity;
          peak = &p;
        }
        if (peak == nullptr){
          continue;
        }
        size++;
        fs << scanNumber << ",0," << pg.maxQScoreCharge
           << "," << log10(pg.perChargeSNR[pg.maxQScoreCharge] + 1e-3) << "," << log10(peak->intensity + 1) << "," << peak->mz
          << "," << pg.monoisotopicMass << "," << log10(pg.totalSNR+1e-3) << "," << pg.isotopeCosineScore
          << "," << pg.chargeCosineScore << "," << log10(pg.intensity+1)
          << "," << pg.qScore
          <<  ",f\n";
      }
    }
  }


  void DeconvolutedSpectrum::writeTopFD(std::fstream &fs, int id)//, fstream &fsm, fstream &fsp)
  {
    if (spec->getMSLevel() == 1 || precursorPeakGroup== nullptr)
    {
      return;
    }
    fs << std::fixed << std::setprecision(2);
    fs << "BEGIN IONS\n"
       << "ID=" << id << "\n"
       << "SCANS=" << scanNumber << "\n"
       << "RETENTION_TIME=" << spec->getRT() << "\n";

      fs << "ACTIVATION=" << activationMethod << "\n";

    fs << "MS_ONE_ID=" << precursorPeakGroup->specIndex << "\n"
       << "MS_ONE_SCAN=" << precursorPeakGroup->scanNumber << "\n"
       << "PRECURSOR_MZ="
       << std::to_string(precursorPeak->mz) << "\n"
       << "PRECURSOR_CHARGE=" << precursorPeak->charge << "\n"
       << "PRECURSOR_MASS=" << std::to_string(precursorPeakGroup->monoisotopicMass) << "\n"
       << "PRECURSOR_INTENSITY=" << precursorPeak->intensity << "\n";
    fs << std::setprecision(-1);


    double scoreThreshold = 0;
    std::vector<double> scores;

    if(peakGroups.size()>500)// max peak count for TopPic
    {
      scores.reserve(peakGroups.size());
      for (auto &pg : peakGroups)
      {
        scores.push_back(pg.isotopeCosineScore);
      }
      std::sort(scores.begin(), scores.end());
      scoreThreshold = scores[scores.size() - 500];
      std::vector<double>().swap(scores);
    }

    int size = 0;
    for (auto &pg : peakGroups)
    {
      if (pg.isotopeCosineScore < scoreThreshold || size >= 500){
        continue;
      }
      size++;
      fs << std::fixed << std::setprecision(2);
      fs << std::to_string(pg.monoisotopicMass) << "\t" << pg.intensity << "\t" << pg.maxQScoreCharge
         //  << "\t" << log10(pg.precursorSNR+1e-10) << "\t" << log10(pg.precursorTotalSNR+1e-10)
         //  << "\t" << log10(pg.maxSNR + 1e-10) << "\t" << log10(pg.totalSNR + 1e-10)
         << "\n";
      fs << std::setprecision(-1);
    }

    fs << "END IONS\n\n";
  }

  bool DeconvolutedSpectrum::registerPrecursor(DeconvolutedSpectrum &precursorSpectrum)
  {
    //precursorSpectrum.updatePeakGroupMap();
    for (auto &p: spec->getPrecursors())
    {
      for (auto &act :  p.getActivationMethods())
      {
        activationMethod = Precursor::NamesOfActivationMethodShort[act];
        break;
      }

      auto startMz = p.getIsolationWindowLowerOffset() > 100.0 ?
                     p.getIsolationWindowLowerOffset() :
                     -p.getIsolationWindowLowerOffset() + p.getMZ();
      auto endMz = p.getIsolationWindowUpperOffset() > 100.0 ?
                   p.getIsolationWindowUpperOffset() :
                   p.getIsolationWindowUpperOffset() + p.getMZ();

      //auto startMz = p.getMZ() - p.getMZ()*1e-5;
      //auto endMz = p.getMZ() + p.getMZ()*1e-5;

      double maxSumIntensity = 0.0;
      for (auto &pg:precursorSpectrum.peakGroups)
      {
        if (pg.peaks[0].mz > endMz || pg.peaks[pg.peaks.size() - 1].mz < startMz)
        {
          continue;
        }

        double sumIntensity = .0;
        double maxIntensity = .0;
        LogMzPeak *tmp = nullptr;
        for (auto &pt:pg.peaks)
        {
          if (pt.mz < startMz)
          {
            continue;
          }
          if (pt.mz > endMz)
          {
            break;
          }
          sumIntensity += pt.intensity;

          if(pt.intensity < maxIntensity){
            continue;
          }
          maxIntensity = pt.intensity;
          tmp = &pt;
        }

        if (sumIntensity <= maxSumIntensity || tmp == nullptr)
        {
          continue;
        }

        maxSumIntensity = sumIntensity;
        precursorPeak = tmp;
        precursorPeakGroup = &pg;
      }

      if (precursorPeak == nullptr)
      {
        double minDistance = 10000.0;
        auto position =
            std::lower_bound(precursorSpectrum.peaks.begin(), precursorSpectrum.peaks.end(), LogMzPeak(startMz)) -
            precursorSpectrum.peaks.begin();

        for (; position < (int) precursorSpectrum.peaks.size(); position++)
        {
          auto mz = precursorSpectrum.peaks[position].mz;
          if (mz < startMz)
          {
            continue;
          }
          if (mz > endMz)
          {
            break;
          }

          auto distance = abs(p.getMZ() - mz);
          if (distance > minDistance)
          {
            continue;
          }

          minDistance = distance;
          precursorPeak = &(precursorSpectrum.peaks[position]);
        }
      }
    }

     return precursorPeakGroup != nullptr;
   }


  double DeconvolutedSpectrum::getQScore() const
  {

    auto pg = precursorPeakGroup;
    auto peak = precursorPeak;
    double score;
    if (peak == nullptr)
    {
      return -5;
    }
    else if (pg == nullptr)
    { // all zero
      score = -1.3923 * log10(peak->intensity + 1) + 8.3952;
    }
    else if (pg->perChargeSNR[peak->charge] <= 0)
    {// partial zero
      score = -1.3234 * log10(peak->intensity + 1)
              - 0.7414 * log10(pg->totalSNR + 1e-3)
              - 0.373 * pg->isotopeCosineScore
              - 0.7726 * pg->chargeCosineScore
              + 0.7945 * log10(pg->intensity + 1)
              + 2.6411;
    }
    else
    {
      score = -0.4318 * log10(pg->perChargeSNR[peak->charge] + 1e-3)
              - 0.2366 * log10(peak->intensity + 1)
              - 1.0932 * log10(pg->totalSNR + 1e-3)
              + 2.8047 * pg->isotopeCosineScore
              - 1.2686 * pg->chargeCosineScore
              - 0.2606 * log10(pg->intensity + 1)
              + 2.325;
    }

    return -score;

    //Variable                       f
    //================================
    //ChargeSNR                -0.4318 Non zero
    //PeakIntensity            -0.2366
    //MassSNR                  -1.0932
    //IsotopeCosine             2.8047
    //ChargeIntensityCosine    -1.2686
    //MassIntensity            -0.2606
    //Intercept                  2.325

    //Variable                       f
    //================================
    //PeakIntensity             -1.3234 partial zero
    //MassSNR                   -0.7414
    //IsotopeCosine              -0.373
    //ChargeIntensityCosine     -0.7726
    //MassIntensity            0.7945
    //Intercept                2.6411


    //Variable               f
    //======================== all zero
    //PeakIntensity    -1.3923
    //Intercept         8.3952
  }

 }