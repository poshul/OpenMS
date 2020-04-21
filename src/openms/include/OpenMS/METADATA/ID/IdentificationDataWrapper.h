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
// $Maintainer: Samuel Wein $
// $Authors: Samuel Wein $
// --------------------------------------------------------------------------

#pragma once

#include <OpenMS/METADATA/ID/IdentificationData.h>

#define PYOPENMS //TODO get this from cmake

namespace OpenMS
{
  /*!
      @brief wrappers for the IdentificationData class to allow pyopenms to work
    */
    class OPENMS_DLLAPI IdentificationDataWrapper: public IdentificationData
    {
     public:
      /*!
        @brief Internal use only
        @return the position of the inputfileref on the IFRefmanager_ vector so it can be wrapped in Cython
        */
      int pythonRegisterInputFile(const String& File);

      /*!
        @brief Internal use only
        @return the position of the processingsoftwareref in the PSoftRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterDataProcessingSoftware(const DataProcessingSoftware& software);

      /*!
        @brief Internal use only
        @return the position of the searchparamref in the DBSRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterDBSearchParam(const DBSearchParam& param);

      /*!
        @brief Internal use only
        @return the position of the ProcessingStepRef in the DPSRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterDataProcessingStep(const DataProcessingStep&
                                          step);

      /*!
        @brief Internal use only
        @return the position of the ProcessingStepRef in the DPSRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterDataProcessingStep(
        const DataProcessingStep& step, int search_ref);

      /*!
        @brief Internal use only
        @return the position of the ScoreTypeRef in the STRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterScoreType(const ScoreType& score);

      /*!
        @brief Internal use only
        @return the position of the DataQueryRef in the DQRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterDataQuery(const DataQuery& query);

      /*!
        @brief Internal use only
        @return the position of the ParentMoleculeRef in the PMRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterParentMolecule(const ParentMolecule& parent);

      /*!
        @brief Internal use only
        @return the position of the IdentifiedPeptideRef in the IPRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterIdentifiedPeptide(const IdentifiedPeptide& peptide);

      /*!
        @brief Internal use only
        @return the position of the IdentifiedCompoundRef in the ICRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterIdentifiedCompound(const IdentifiedCompound& compound);

      /*!
        @brief Internal use only
        @return the position of the IdentifiedOligoRef in the IORefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterIdentifiedOligo(const IdentifiedOligo& oligo);

      /*!
        @brief Internal use only
        @return the position of the QueryMatchRef in the QMRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterMoleculeQueryMatch(const MoleculeQueryMatch& match);

      /*!
        @brief Internal use only
        @return the position of the MatchGroupRef in the MGRefmanager_ vector so it can be wrapped in python
      */
      int pythonRegisterQueryMatchGroup(const QueryMatchGroup& group);
    
    protected:
      // SPW: Vector to store all the IteratorWrappers for use with pyOpenMS code
      std::vector<InputFileRef> IFRefManager_;
      std::vector<ProcessingSoftwareRef> PSoftRefManager_;
      std::vector<ProcessingStepRef> PStepRefManager_;
      std::vector<SearchParamRef> DBSRefManager_;
      std::vector<ScoreTypeRef> STRefManager_;
      std::vector<DataQueryRef> DQRefManager_;
      std::vector<ParentMoleculeRef> PMRefManager_;
      std::vector<IdentifiedPeptideRef> IDPRefManager_;
      std::vector<IdentifiedCompoundRef> IDCRefManager_;
      std::vector<IdentifiedOligoRef> IDORefManager_;
      std::vector<IdentifiedMoleculeRef> IDMRefManager_;
      std::vector<QueryMatchRef> QMRefManager_;
      std::vector<MatchGroupRef> MGRefManager_;
  };
} // namespace OpenMS