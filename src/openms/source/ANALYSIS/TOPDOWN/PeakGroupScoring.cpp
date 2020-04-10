//
// Created by Kyowon Jeong on 10/8/19.
//

#include "OpenMS/ANALYSIS/TOPDOWN/PeakGroupScoring.h"

namespace OpenMS
{

  PeakGroupScoring::PeakGroupScoring(std::vector<PeakGroup> &pg, Parameter &p) :
      peakGroups(pg), param(p)
  {
  }

  PeakGroupScoring::~PeakGroupScoring()
  {

  }

  double PeakGroupScoring::getAvgMassPpmError(PeakGroup &pg)
  {
    double sum = .0;
    // double maxMass = pg.monoisotopicMass;//maxIntensityPeak.getUnchargedMass() - maxIntensityPeak.isotopeIndex * Constants::ISOTOPE_MASSDIFF_55K_U;

    for (auto &p : pg.peaks)
    {
      auto tm = pg.monoisotopicMass + p.isotopeIndex * Constants::ISOTOPE_MASSDIFF_55K_U;
      auto diff = tm - p.getUnchargedMass();
      diff /= tm;
      diff = diff * diff;
      sum += diff;
    }
    return (sum / pg.peaks.size());
  }

  double PeakGroupScoring::getChargeFitScore(double *perChargeIntensity, int range)
  {
    double maxPerChargeIntensity = .0;
    std::vector<double> xs;
    std::vector<double> ys;

    xs.reserve(range + 2);
    ys.reserve(range + 2);

    for (int i = 0; i < range; i++)
    {
      maxPerChargeIntensity = std::max(maxPerChargeIntensity, perChargeIntensity[i]);
    }

    double th = maxPerChargeIntensity * .02;// as recommended in the original paper...
    int first = -1, last = 0;
    for (int i = 0; i < range; i++)
    {
      if (perChargeIntensity[i] <= th)
      {
        continue;
      }
      if (first < 0)
      {
        first = i;
      }

      last = i;
    }

    for (int i = first; i <= last; i++)
    {
      xs.push_back(i);
      ys.push_back((1 + perChargeIntensity[i]));
    }

    if (xs.size() <= 3)
    {
      return 0.5;
    }

    Eigen::Matrix3d m;
    Eigen::Vector3d v;

    double s0 = 0, s1 = 0, s2 = 0, s3 = 0, s4 = 0;
    double t0 = 0, t1 = 0, t2 = 0;

    for (Size i = 0; i < xs.size(); i++)
    {
      auto &x = xs[i];
      auto y = log(ys[i]);
      s0++;
      s1 += x;
      s2 += x * x;
      s3 += x * x * x;
      s4 += x * x * x * x;
      t0 += y;
      t1 += y * x;
      t2 += y * x * x;
    }
    m(0, 0) = s0;
    m(1, 0) = m(0, 1) = s1;
    m(2, 0) = m(1, 1) = m(0, 2) = s2;
    m(2, 1) = m(1, 2) = s3;
    m(2, 2) = s4;

    auto im = m.inverse();
    v(0) = t0;
    v(1) = t1;
    v(2) = t2;
    //cout<<v<<endl;
    auto abc = im * v;
    //cout<<abc<<endl;
    double mu = -abc(1) / abc(2) / 2;
    double omega = -1 / abc(2) / 2;

    if (omega <= 0)
    {
      return 0;
    }
    std::vector<double> tys;

    for (Size i = 0; i < ys.size(); i++)
    {
      double ty = exp(-(xs[i] - mu) * (xs[i] - mu) / 2 / omega);
      tys.push_back(ty);
    }
    return getCosine(ys, tys);
  }

  double
  PeakGroupScoring::getCorrelation(const double *a,
                                   int &aStart,
                                   int &aEnd,
                                   IsotopeDistribution &b,
                                   int &bSize,
                                   int offset)
  {
    double n = .0, d1 = .0;
    double ma = .0, mb = .0;
    for (int j = aStart; j < aEnd; j++)
    {
      ma += a[j];
    }
    ma /= (aEnd - aStart);

    for (int j = 0; j < bSize; j++)
    {
      mb += b[j].getIntensity();
    }
    mb /= bSize;

    double bNorm = .0;
    for (int j = 0; j < bSize; j++)
    {
      bNorm += (b[j].getIntensity() - mb) * (b[j].getIntensity() - mb);
    }

    for (int j = aStart; j < aEnd; j++)
    {
      d1 += (a[j] - ma) * (a[j] - ma);
      int i = j - offset;
      if (i < 0 || i >= bSize)
      {
        continue;
      }
      n += (a[j] - ma) * (b[i].getIntensity() - mb); //
    }

    double d = (d1 * bNorm);
    if (d <= 0)
    {
      return 0;
    }
    return n / sqrt(d);
  }


  double
  PeakGroupScoring::getCosine(const double *a,
                              int &aStart,
                              int &aEnd,
                              IsotopeDistribution &b,
                              int &bSize,
                              double &bNorm,
                              int offset)
  {
    double n = .0, d1 = .0;
    int c = 0;
    for (int j = aStart; j <= aEnd; j++)
    {
      d1 += a[j] * a[j];
      int i = j - offset;
      if (i < 0 || i >= bSize)
      {
        continue;
      }
      c++;
      n += a[j] * b[i].getIntensity(); //
    }
    if (c < 2)
    {
      return 0;
    }
    double d = (d1 * bNorm);
    if (d <= 0)
    {
      return 0;
    }
    return n / sqrt(d);
  }

  double PeakGroupScoring::getCosine(const double *a, double *b, Size size)
  {
    double n = .0, d1 = .0, d2 = .0;
    //int overlapCntr = 0;
    for (Size j = 0; j < size; j++)
    {
      d1 += a[j] * a[j];
      d2 += b[j] * b[j];
      n += a[j] * b[j];
      //  if(a[j] > 0 && b[j] > 0) overlapCntr++;
    }

    //if(overlapCntr < 2) return 0; //
    double d = (d1 * d2);
    if (d <= 0)
    {
      return 0;
    }
    return n / sqrt(d);
  }


  double PeakGroupScoring::getCosine(std::vector<double> &a, std::vector<double> &b, int off)
  {
    double n = .0, d1 = .0, d2 = .0;
    Size size = a.size();
    //int overlapCntr = 0;
    for (Size j = off; j < size - off; j++)
    {
      d1 += a[j] * a[j];
      d2 += b[j] * b[j];
      n += a[j] * b[j];
      //  if(a[j] > 0 && b[j] > 0) overlapCntr++;
    }

    //if(overlapCntr < 2) return 0; //
    double d = (d1 * d2);
    if (d <= 0)
    {
      return 0;
    }
    return n / sqrt(d);
  }

  double PeakGroupScoring::getIsotopeCosineAndDetermineIsotopeIndex(double mass,
                                                                    double *perIsotopeIntensities,
                                                                    int perIsotopeIntensitiesSize,
                                                                    int &offset,
                                                                    FLASHDeconvHelperStructs::PrecalcularedAveragine &avg)
  {
    auto iso = avg.get(mass);
    auto isoNorm = avg.getNorm(mass);

    int isoSize = (int) iso.size();

    offset = 0;
    double maxCosine = -1;
    int isotopeLength = 0;
    int maxIsotopeIndex = 0, minIsotopeIndex = -1;

    for (int i = 0; i < perIsotopeIntensitiesSize; i++)
    {
      if (perIsotopeIntensities[i] <= 0)
      {
        continue;
      }
      isotopeLength++;
      maxIsotopeIndex = i;
      if (minIsotopeIndex < 0)
      {
        minIsotopeIndex = i;
      }
    }


    for (int f = -isoSize - minIsotopeIndex; f <= maxIsotopeIndex; f++)
    {
      auto cos = PeakGroupScoring::getCosine(perIsotopeIntensities,
                                             minIsotopeIndex,
                                             maxIsotopeIndex,
                                             iso,
                                             isoSize,
                                             isoNorm,
                                             f);

      /* auto cos = PeakGroupScoring::getCorrelation(perIsotopeIntensities,
                                              minIsotopeIndex,
                                              maxIsotopeIndex,
                                              iso,
                                              isoSize,
                                              f);
 */
      if (maxCosine < cos)
      {
        maxCosine = cos;
        offset = f;
      }
    }

    return maxCosine;
  }


  bool PeakGroupScoring::checkChargeDistribution(double *perChargeIntensity,
                                                 int range,
                                                 int threshold
  )
  {
    double maxPerChargeIntensity = .0;
    int nonZeroStart = -1, nonZeroEnd = 0;
    for (int i = 0; i < range; i++)
    {
      if (perChargeIntensity[i] > 0)
      {
        maxPerChargeIntensity = std::max(maxPerChargeIntensity, perChargeIntensity[i]);
        if (nonZeroStart < 0)
        {
          nonZeroStart = i;
        }
        nonZeroEnd = i;
      }
    }

    int prevCharge = nonZeroStart;

    int n_r = 0;
    double factor = 4.0;
    double intensityThreshold = maxPerChargeIntensity / factor;//intensities[intensities.size()*95/100] / 5.0;
    for (int k = prevCharge + 1; k <= nonZeroEnd; k++)
    {
      if (perChargeIntensity[k] <= intensityThreshold)
      {
        continue;
      }

      if (k - prevCharge == 1)
      {
        n_r++;
      }
      //maxn_r = maxn_r < n_r? n_r : maxn_r;
      if (n_r >= threshold)
      {
        return true;
      }
      prevCharge = k;
    }

    return false;

    /*for (int i = 2; i < std::min(7, range); i++)
    {
      for (int l = 0; l < i; l++)
      {
        int t = 0;
        int maxt = 0;
        prevCharge = nonZeroStart + l;
        for (int k = prevCharge + i; k <= nonZeroEnd; k += i)
        {if (perChargeIntensity[k] <= intensityThreshold)
          {
            continue;
          }
          if (k - prevCharge == i)
          {
            t++;
          }{
            t = 0;
          }
          maxt = maxt < t ? t : maxt;
          prevCharge = k;
        }
        if (n_r <= maxt)
        {
          return false;
        }
      }
    }*/
  }

  std::vector<FLASHDeconvHelperStructs::PeakGroup> &PeakGroupScoring::scoreAndFilterPeakGroups(unsigned int &msLevel,
                                                                                               FLASHDeconvHelperStructs::PrecalcularedAveragine &avg)
  {
    std::vector<FLASHDeconvAlgorithm::PeakGroup> filteredPeakGroups;
    filteredPeakGroups.reserve(peakGroups.size());
    double threshold = .0;

    Size mc = (Size) param.maxMassCount;
    if (mc > 0)
    {
      std::vector<double> intensities;
      intensities.reserve(peakGroups.size());

      for (auto &pg : peakGroups)
      {
        pg.updateMassesAndIntensity(avg);
        intensities.push_back(pg.intensity);
      }

      if (intensities.size() > mc)
      {
        sort(intensities.begin(), intensities.end());
        threshold = intensities[intensities.size() - mc];
      }
      std::vector<double>().swap(intensities);
    }

    auto perIsotopeIntensity = new double[param.maxIsotopeCount];
    auto perChargeIntensity = new double[param.currentChargeRange];

    for (auto &pg : peakGroups)
    {
      if (pg.intensity < threshold)
      {
        //delete[] pg.perChargeSNR;
        continue; //
      }

      auto indices = updatePerChargeIsotopeIntensity(
          //          intensityGrid,intensityGrid2,
          perIsotopeIntensity, perChargeIntensity,
          pg);

      pg.chargeCosineScore = getChargeFitScore(perChargeIntensity, param.currentChargeRange);

      if (msLevel == 1)
      {
        if (pg.peaks.empty() ||
            pg.chargeCosineScore <= param.minChargeCosine)
        {
         // delete[] pg.perChargeSNR;
          continue;
        }

        bool isChargeWellDistributed = checkChargeDistribution(perChargeIntensity,
                                                               param.chargeRange,
                                                               param.minContinuousChargePeakCount[msLevel -1]);

        if (!isChargeWellDistributed)
        {
          //delete[] pg.perChargeSNR;
          continue;
        }

      }
      else
      {
        if (pg.peaks.empty()|| pg.chargeCosineScore < 0.1)//
        {
         // delete[] pg.perChargeSNR;
          continue;
        }
      }

      int offset = 0;
      pg.isotopeCosineScore = getIsotopeCosineAndDetermineIsotopeIndex(pg.peaks[0].getUnchargedMass(),
                                                                       perIsotopeIntensity,
                                                                       param.maxIsotopeCount,
                                                                       offset,
                                                                       avg);

      if (pg.peaks.empty() ||
          (pg.isotopeCosineScore <= param.minIsotopeCosine[msLevel-1]))// (msLevel <= 1 ? param.minIsotopeCosineSpec : param.minIsotopeCosineSpec2)))
      {
        //delete[] pg.perChargeSNR;
        continue;
      }

      pg.updateMassesAndIntensity(avg, offset, param.maxIsotopeCount);

      //// TODO test codes..
      auto iso = avg.get(pg.monoisotopicMass);
      auto isoNorm = avg.getNorm(pg.monoisotopicMass);
      int isoSize = (int) iso.size();

      double maxSNR = 0;
      for(auto charge=pg.minCharge;charge<=pg.maxCharge;charge++){
        auto j = charge - param.minCharge;
        auto perIsotopeIntensities = new double[param.maxIsotopeCount];
        std::fill_n(perIsotopeIntensities,param.maxIsotopeCount,.0);

        int minIsotopeIndex = param.maxIsotopeCount;
        int maxIsotopeIndex = 0;

        double minMz = pg.monoisotopicMass * 2;
        double maxMz = 0;

        for(auto& p:pg.peaks){
          if(p.charge != charge){
            continue;
          }
          //sp += p.intensity * p.intensity;
          perIsotopeIntensities[p.isotopeIndex] += p.intensity;
          minIsotopeIndex = minIsotopeIndex < p.isotopeIndex ? minIsotopeIndex : p.isotopeIndex;
          maxIsotopeIndex = maxIsotopeIndex < p.isotopeIndex ? p.isotopeIndex : maxIsotopeIndex;

          minMz = minMz < p.mz ? minMz : p.mz;
          maxMz = maxMz > p.mz ? maxMz : p.mz;
        }
        if(maxMz <= 0){
          delete[] perIsotopeIntensities;
          continue;
        }

        double sp = .0;
        for (int k = minIsotopeIndex; k <=maxIsotopeIndex ; ++k)
        {
          if(k>isoSize){
            break;
          }
          sp += perIsotopeIntensities[k] * perIsotopeIntensities[k];
        }

        auto cos = PeakGroupScoring::getCosine(perIsotopeIntensities,
                                               minIsotopeIndex,
                                               maxIsotopeIndex,
                                               iso,
                                               isoSize,
                                               isoNorm,
                                               0);

        double cos2 = cos * cos;
        pg.perChargeSNR[j] = cos2 * sp / ((1- cos2) * sp + pg.perChargeSNR[j] + 1);

        if(pg.perChargeSNR[j] > maxSNR){
          maxSNR = pg.perChargeSNR[j];
          pg.maxSNR = pg.perChargeSNR[j];
          pg.maxSNRcharge = charge;
          pg.maxSNRminMz = minMz;
          pg.maxSNRmaxMz = maxMz;
        }

        delete[] perIsotopeIntensities;
      }
      //delete[] pg.perChargeSNR;
      if(pg.maxSNR>0)
      {
        filteredPeakGroups.push_back(pg);
      }
    }

    //std::cout<<filteredPeakGroups.size();
    peakGroups.swap(filteredPeakGroups);

    std::vector<PeakGroup>().swap(filteredPeakGroups);

    //filterPeakGroupsByIsotopeCosine(param.currentMaxMassCount);
    removeOverlappingPeakGroups(param.tolerance[msLevel-1]);

    delete[] perIsotopeIntensity;
    delete[] perChargeIntensity;

    return peakGroups;
  }

  void PeakGroupScoring::filterPeakGroupsByIsotopeCosine(int currentMaxMassCount)
  {
    if (currentMaxMassCount < 0 || peakGroups.size() <= (Size) currentMaxMassCount)
    {
      return;
    }
    Size mc = (Size) currentMaxMassCount;
    std::vector<double> scores;
    for (auto &pg : peakGroups)
    {
      scores.push_back(pg.isotopeCosineScore);
    }

    sort(scores.begin(), scores.end());

    auto threshold = scores[scores.size() - mc];
    for (auto pg = peakGroups.begin(); pg != peakGroups.end();)
    {
      if (peakGroups.size() <= mc)
      {
        break;
      }
      if (pg->isotopeCosineScore < threshold)
      {
        pg = peakGroups.erase(pg);
        continue;
      }
      ++pg;
    }
  }

  void PeakGroupScoring::removeOverlappingPeakGroups(double tol)
  { // pgs are sorted
    //return;
    sort(peakGroups.begin(), peakGroups.end());
    std::vector<PeakGroup> merged;
    merged.reserve(peakGroups.size());

    for (Size i = 0; i < peakGroups.size(); i++)
    {
      bool select = true;
      auto &pg = peakGroups[i];

      if (pg.monoisotopicMass <= 0)
      {
        continue;
      }
      if (i > 0 && pg.monoisotopicMass == peakGroups[i - 1].monoisotopicMass)
      {
        continue;
      }

      double massTol = pg.monoisotopicMass * tol;

      for (Size j = i + 1; j < peakGroups.size(); j++)
      {
        auto &pgo = peakGroups[j];
        if (!select || pgo.monoisotopicMass - pg.monoisotopicMass > massTol)
        {
          break;
        }
        select &= pg.isotopeCosineScore >= pgo.isotopeCosineScore;
      }

      if (!select)
      {
        continue;
      }
      for (int j = (int) i - 1; j >= 0; j--)
      {
        auto &pgo = peakGroups[j];
        if (!select || pg.monoisotopicMass - pgo.monoisotopicMass > massTol)
        {
          break;
        }
        select &= pg.isotopeCosineScore >= pgo.isotopeCosineScore;
      }
      if (!select)
      {
        continue;
      }
      merged.push_back(pg);
    }
    //pgs = merged;
    //merged.shrink_to_fit();
    std::vector<PeakGroup>().swap(peakGroups);
    merged.swap(peakGroups);
    //vector<PeakGroup>().swap(merged);
  }


  std::vector<int> PeakGroupScoring::updatePerChargeIsotopeIntensity(
      //      double **intensityGrid,
      //      double **intensityGrid2,
      double *perIsotopeIntensity,
      double *perChargeIntensity,
      PeakGroup &pg)
  {
    std::fill_n(perIsotopeIntensity, param.maxIsotopeCount, 0);
    std::fill_n(perChargeIntensity, param.currentChargeRange, 0);
    /*for (int j = 0; j < param.currentChargeRange; ++j)
    {
      std::fill_n(intensityGrid[j], param.maxIsotopeCount, 0);
    }

    for (int j = 0; j < param.maxIsotopeCount; ++j)
    {
      std::fill_n(intensityGrid2[j], param.currentChargeRange, 0);
    }*/

    int minCharge = param.currentChargeRange + param.minCharge + 1;
    int maxCharge = 0;
    //    double maxIntensity = -1;
    int maxIntChargeIndex = -1;
    //    double maxIntensity2 = -1;
    int maxIntIsoIndex = -1;

    for (auto &p : pg.peaks)
    {
      if (p.isotopeIndex < 0 || p.isotopeIndex >= param.maxIsotopeCount)
      {
        continue;
      }
      minCharge = std::min(minCharge, p.charge);
      maxCharge = std::max(maxCharge, p.charge);

      int index = p.charge - param.minCharge;
      perIsotopeIntensity[p.isotopeIndex] += p.intensity;
      perChargeIntensity[index] += p.intensity;
      //intensityGrid[index][p.isotopeIndex] += p.intensity;
      //intensityGrid2[p.isotopeIndex][index] += p.intensity;

      /*if(maxIntensity < intensityGrid[index][p.isotopeIndex]){
        maxIntensity = intensityGrid[index][p.isotopeIndex];
        maxIntChargeIndex = index;
      }

      if(maxIntensity2 < intensityGrid2[p.isotopeIndex][index]){
        maxIntensity2 = intensityGrid2[p.isotopeIndex][index];
        maxIntIsoIndex = p.isotopeIndex;
      }*/
    }
    pg.maxCharge = maxCharge;
    pg.minCharge = minCharge;

    return std::vector<int>{maxIntChargeIndex, maxIntIsoIndex};
  }

}
