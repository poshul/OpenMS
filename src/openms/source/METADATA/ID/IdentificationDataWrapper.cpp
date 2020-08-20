// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2020.
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
// $Maintainer: Hendrik Weisser $
// $Authors: Hendrik Weisser $
// --------------------------------------------------------------------------

#include <OpenMS/METADATA/ID/IdentificationDataWrapper.h>


using namespace std;

namespace OpenMS
{
  
  int IdentificationDataWrapper::pythonRegisterInputFile(const String& file)
  {
    auto result = IdentificationData::input_files_.insert(file) ;
    if (result.second) // If the insert added a new element
    {
      IFRefManager_.push_back(result.first);
      return IFRefManager_.size();
    }
    else // else the element already exists. Lets find it.
    {
      auto it = std::find(IFRefManager_.begin(), IFRefManager_.end(), result.first);
      return std::distance(IFRefManager_.begin(), it);
    }
  }

  int IdentificationDataWrapper::pythonRegisterDataProcessingSoftware(
    const IdentificationData::DataProcessingSoftware& software)
  {
    for (ScoreTypeRef score_ref : software.assigned_scores)
    {
      if (!isValidReference_(score_ref, score_types_))
      {
        String msg = "invalid reference to a score type - register that first";
        throw Exception::IllegalArgument(__FILE__, __LINE__,
                                         OPENMS_PRETTY_FUNCTION, msg);
      }
    }
    auto result = processing_softwares_.insert(software);
    if (result.second) // If the insert added a new element
    {
      PSoftRefManager_.push_back(result.first);
      return PSoftRefManager_.size();
    }
    else // else the element already exists. Lets find it.
    {
      auto it = std::find(PSoftRefManager_.begin(), PSoftRefManager_.end(), result.first);
      return std::distance(PSoftRefManager_.begin(), it);
    }
  }

  int IdentificationDataWrapper::pythonRegisterDBSearchParam(const IdentificationData::DBSearchParam& param)
  {
    auto result = db_search_params_.insert(param);
    if (result.second) // If the insert added a new element
    {
      DBSRefManager_.push_back(result.first);
      return DBSRefManager_.size();
    }
    else // else the element already exists. Lets find it.
    {
      auto it = std::find(DBSRefManager_.begin(), DBSRefManager_.end(), result.first);
      return std::distance(DBSRefManager_.begin(), it);
    }
  
  }

  int IdentificationDataWrapper::pythonRegisterDataProcessingStep(
    const DataProcessingStep& step)
  {
    return pythonRegisterDataProcessingStep(step, -1); //TODO Check that using -1 as an equivalent to set::end doesn't break anything.
  }
 
  int IdentificationDataWrapper::pythonRegisterDataProcessingStep( const IdentificationData::DataProcessingStep& step, int search_ref)
  {
    // valid reference to software is required:
    if (!isValidReference_(step.software_ref, processing_softwares_))
    {
      String msg = "invalid reference to data processing software - register that first";
      throw Exception::IllegalArgument(__FILE__, __LINE__,
                                       OPENMS_PRETTY_FUNCTION, msg);
    }
    // if given, references to input files must be valid:
    for (InputFileRef ref : step.input_file_refs)
    {
      if (!isValidReference_(ref, input_files_))
      {
        String msg = "invalid reference to input file - register that first";
        throw Exception::IllegalArgument(__FILE__, __LINE__,
                                         OPENMS_PRETTY_FUNCTION, msg);
      }
    }

    int step_ref = -1;
    auto result = processing_steps_.insert(step);
    if (result.second) // If the insert added a new element
    {
      PStepRefManager_.push_back(result.first);
      step_ref = PStepRefManager_.size();
    }
    else // else the element already exists. Lets find it.
    {
      auto it = std::find(PStepRefManager_.begin(), PStepRefManager_.end(), result.first);
      step_ref = std::distance(PStepRefManager_.begin(), it);
    }


    // if given, reference to DB search param. must be valid:
    if (search_ref != -1)  //TODO Check that using -1 as an equivalent to set::end doesn't break anything.
    {
      if (!isValidReference_(DBSRefManager_[search_ref], db_search_params_))
      {
        String msg = "invalid reference to database search parameters - register those first";
      throw Exception::IllegalArgument(__FILE__, __LINE__,
                                       OPENMS_PRETTY_FUNCTION, msg);
      }
      db_search_steps_.insert(make_pair(PStepRefManager_[step_ref], DBSRefManager_[search_ref]));
    }
    return step_ref;
  }
  
  int IdentificationDataWrapper::pythonRegisterScoreType(const ScoreType& score)
  {
    if (score.cv_term.getAccession().empty() && score.cv_term.getName().empty())
    {
      String msg = "score type must have an accession or a name";
      throw Exception::IllegalArgument(__FILE__, __LINE__,
                                       OPENMS_PRETTY_FUNCTION, msg);
    }
    pair<ScoreTypes::iterator, bool> result;
    result = score_types_.insert(score);
    if (!result.second)
    { 
      if (score.higher_better != result.first->higher_better)
      {
        String msg = "score type already exists with opposite orientation";
        throw Exception::IllegalArgument(__FILE__, __LINE__,
                                        OPENMS_PRETTY_FUNCTION, msg);
      }
      else // else the element already exists (with the same orientation) we should have a ref to it in the manager. Lets find it.
      {
        auto it = std::find(STRefManager_.begin(), STRefManager_.end(), result.first);
        return std::distance(STRefManager_.begin(), it);
      }
    }
    else
    {
    STRefManager_.push_back(result.first);
    return STRefManager_.size();
    }
  }

  int IdentificationDataWrapper::pythonRegisterDataQuery(const DataQuery& query)
  {
    // reference to spectrum or feature is required:
    if (query.data_id.empty())
    {
      String msg = "missing identifier in data query";
      throw Exception::IllegalArgument(__FILE__, __LINE__,
                                       OPENMS_PRETTY_FUNCTION, msg);
    }
    // ref. to input file may be missing, but must otherwise be valid:
    if (query.input_file_opt && !isValidReference_(*query.input_file_opt,
                                                   input_files_))
    {
      String msg = "invalid reference to an input file - register that first";
      throw Exception::IllegalArgument(__FILE__, __LINE__,
                                       OPENMS_PRETTY_FUNCTION, msg);
    }
    auto result = data_queries_.insert(query);
    DataQueryRef ref = result.first;
    data_query_lookup_.insert(ref);
    
    if (result.second) // If the insert added a new element
    {
      DQRefManager_.push_back(result.first);
      return DQRefManager_.size();
    }
    else // else the element already exists. Lets find it.
    {
      auto it = std::find(DQRefManager_.begin(), DQRefManager_.end(), result.first);
      return std::distance(DQRefManager_.begin(), it);
    }
  }

  int IdentificationDataWrapper::pythonRegisterIdentifiedPeptide(const IdentifiedPeptide&
                                                          peptide)
  {
    auto result = IdentificationData::registerIdentifiedPeptide(peptide);
    auto it = std::find(IDPRefManager_.begin(), IDPRefManager_.end(), result);
    if (it==IDPRefManager_.end())
    {
      IDPRefManager_.push_back(result);
      return IDPRefManager_.size();
    } 
    else
    {
      return std::distance(IDPRefManager_.begin(), it);
    }
  }

  int IdentificationDataWrapper::pythonRegisterIdentifiedCompound(const IdentifiedCompound&
                                                          compound)
  {
    auto result = IdentificationData::registerIdentifiedCompound(compound);
    auto it = std::find(IDCRefManager_.begin(), IDCRefManager_.end(), result);
    if (it==IDCRefManager_.end())
    {
      IDCRefManager_.push_back(result);
      return IDCRefManager_.size();
    } 
    else
    {
      return std::distance(IDCRefManager_.begin(), it);
    }
  }

  int IdentificationDataWrapper::pythonRegisterIdentifiedOligo(const IdentifiedOligo&
                                                        oligo)
  {
    auto result = IdentificationData::registerIdentifiedOligo(oligo);
    auto it = std::find(IDORefManager_.begin(), IDORefManager_.end(), result);
    if (it==IDORefManager_.end())
    {
      IDORefManager_.push_back(result);
      return IDORefManager_.size();
    } 
    else
    {
      return std::distance(IDORefManager_.begin(), it);
    }
  }
 
  int IdentificationDataWrapper::pythonRegisterParentMolecule(const ParentMolecule& parent)
  {
    auto result = IdentificationData::registerParentMolecule(parent);
    auto it = std::find(PMRefManager_.begin(), PMRefManager_.end(), result);
    if (it==PMRefManager_.end())
    {
      PMRefManager_.push_back(result);
      return PMRefManager_.size();
    } 
    else
    {
      return std::distance(PMRefManager_.begin(), it);

    }
  }
 
  int IdentificationDataWrapper::pythonRegisterMoleculeQueryMatch(const MoleculeQueryMatch& match)
  {
    auto result = registerMoleculeQueryMatch(match);
    auto it = std::find(QMRefManager_.begin(), QMRefManager_.end(), result);
    if (it==QMRefManager_.end())
    {
      QMRefManager_.push_back(result);
      return QMRefManager_.size();
    } 
    else
    {
      return std::distance(QMRefManager_.begin(), it);
    }
  }

 int IdentificationDataWrapper::pythonRegisterQueryMatchGroup(const QueryMatchGroup& group)
 {
    auto result = registerQueryMatchGroup(group);
    auto it = std::find(MGRefManager_.begin(), MGRefManager_.end(), result);
    if (it==MGRefManager_.end())
    {
      MGRefManager_.push_back(result);
      return MGRefManager_.size();
    } 
    else
    {
      return std::distance(MGRefManager_.begin(), it);
    }
 }

 IdentificationDataInternal::DataProcessingSoftware IdentificationDataWrapper::pythonNewDataProcessingSoftware(const String& name = "", const String& version = "", std::vector<int> assigned_scores = std::vector<int>())
  {
    std::vector<IdentificationData::ScoreTypeRef> reffedscores = std::vector<IdentificationData::ScoreTypeRef>();
    for (size_t i=0; i<assigned_scores.size(); i++)
    {
      reffedscores.push_back(STRefManager_[assigned_scores[i]]);
    }
    auto result = DataProcessingSoftware(name, version, reffedscores);
    return result;
  }

IdentificationDataInternal::DataProcessingStep IdentificationDataWrapper::pythonNewDataProcessingStep(int software_ref,
                                                const std::vector<int>& input_file_refs = std::vector<int>(),
                                                const std::vector<String>& primary_files = std::vector<String>(),
                                                const DateTime& date_time = DateTime::now(),
                                                std::set<DataProcessing::ProcessingAction> actions = std::set<DataProcessing::ProcessingAction>())
  {
    ProcessingSoftwareRef pref = PSoftRefManager_[software_ref];
    std::vector<InputFileRef> ifref = std::vector<InputFileRef>();
    for (size_t i = 0; i < input_file_refs.size(); ++i)
    {
      ifref.push_back(IFRefManager_[input_file_refs[i]]);
    }
    auto result = DataProcessingStep(pref, ifref, primary_files, date_time, actions);
    return result;
  }


// ScoreType& pythonNewScoreType(); constructor doesnt need any refs

IdentificationDataInternal::DataQuery IdentificationDataWrapper::pythonNewDataQuery(const String& data_id,
                              int input_file_opt = -1, //We use -1 for not defined, ie boost::none
                              double rt = std::numeric_limits<double>::quiet_NaN(),
                              double mz = std::numeric_limits<double>::quiet_NaN())
  {
    boost::optional<InputFileRef> inputopt = boost::none;
    if (input_file_opt != -1)
      {
        inputopt = IFRefManager_[input_file_opt];
      }
    auto result = DataQuery(data_id, inputopt, rt, mz);
    return result;
  }

IdentificationDataInternal::ParentMolecule IdentificationDataWrapper::pythonNewParentMolecule(const String& accession,
                                        MoleculeType molecule_type = MoleculeType::PROTEIN,
                                        const String& sequence = "",
                                        const String& description = "",
                                        double coverage = 0.0,
                                        bool is_decoy = false)//,
                                        // AppliedProcessingSteps& steps_and_scores); //SPW this is tricky, TODO implement it
  {
    auto result = ParentMolecule(accession, molecule_type, sequence, description, coverage, is_decoy);
    return result;
  }

IdentificationDataInternal::IdentifiedPeptide IdentificationDataWrapper::pythonNewIdentifiedPeptide(AASequence seq)//,
                                                //const ParentMatches& parent_matches,  //TODO
                                                //const AppliedProcessingSteps& steps_and_scores);
  {
    auto result = IdentificationDataInternal::IdentifiedSequence<AASequence>(seq);
    return result;
  }

IdentificationDataInternal::IdentifiedOligo IdentificationDataWrapper::pythonNewIdentifiedOligo(NASequence seq)//,
                                                //const ParentMatches& parent_matches,  //TODO
                                                //const AppliedProcessingSteps& steps_and_scores);
  {
    auto result = IdentificationDataInternal::IdentifiedSequence<NASequence>(seq);
    return result;
  }

//TODO add specific functions for the different types?

IdentificationDataInternal::IdentifiedCompound IdentificationDataWrapper::pythonNewIdentifiedCompound(const String& identifier,
                                                const EmpiricalFormula& formula = EmpiricalFormula(),
                                                const String& name = "", const String& smile = "",
                                                const String& inchi = "")// const AppliedProcessingSteps& steps_and_scores = AppliedProcessingSteps()); //TODO Implement this
  {
    auto result = IdentifiedCompound(identifier, formula, name, smile, inchi);
    return result;
  }

IdentificationDataInternal::MoleculeQueryMatch IdentificationDataWrapper::pythonNewMolecularQueryMatch(int identified_molecule_ref,
                                                  int data_query_ref, int m_charge = 0)//,
                                                  //const AppliedProcessingSteps& steps_and_scores, //TODO implement these
                                                  //const PeakAnnotationSteps& peak_annotations);
  {
    auto result = MoleculeQueryMatch(IDMRefManager_[identified_molecule_ref], DQRefManager_[data_query_ref], m_charge);
    return result;
  }

IdentificationDataInternal::QueryMatchGroup IdentificationDataWrapper::pythonNewQueryMatchGroup(std::vector<int> qmr)
  {
    QueryMatchGroup result = QueryMatchGroup();
    for (size_t i = 0; i < qmr.size(); ++i)
    {
      result.query_match_refs.insert(QMRefManager_[qmr[i]]);
    }
    return result;
  }




/*
  IdentificationData::ProcessingStepRef
  IdentificationData::getCurrentProcessingStep()
  {
    return current_step_ref_;
  }

  pair<IdentificationData::ScoreTypeRef, bool>
  IdentificationData::findScoreType(const String& score_name) const
  {
    for (ScoreTypeRef it = score_types_.begin(); it != score_types_.end(); ++it)
    {
      if (it->cv_term.getName() == score_name)
      {
        return make_pair(it, true);
      }
    }
    return make_pair(score_types_.end(), false);
  }


  vector<IdentificationData::QueryMatchRef>
  IdentificationData::getBestMatchPerQuery(ScoreTypeRef score_ref) const
  {
    vector<QueryMatchRef> results;
    bool higher_better = score_ref->higher_better;
    pair<double, bool> best_score = make_pair(0.0, false);
    QueryMatchRef best_ref = query_matches_.end();
    for (QueryMatchRef ref = query_matches_.begin();
         ref != query_matches_.end(); ++ref)
    {
      pair<double, bool> current_score = ref->getScore(score_ref);
      if ((best_ref != query_matches_.end()) &&
          (ref->data_query_ref != best_ref->data_query_ref))
      {
        // finalize previous query:
        if (best_score.second) results.push_back(best_ref);
        best_score = current_score;
        best_ref = ref;
      }
      else if (current_score.second &&
               (!best_score.second ||
                isBetterScore(current_score.first, best_score.first,
                              higher_better)))
      {
        // new best score for the current query:
        best_score = current_score;
        best_ref = ref;
      }
    }
    // finalize last query:
    if (best_score.second) results.push_back(best_ref);

    return results;
  }
*/

} // end namespace OpenMS
