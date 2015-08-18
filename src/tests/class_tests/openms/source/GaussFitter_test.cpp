// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry               
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2015.
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
// $Maintainer: Andreas Bertsch $
// $Authors: Andreas Bertsch $
// --------------------------------------------------------------------------

#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/test_config.h>

///////////////////////////
#include <OpenMS/MATH/STATISTICS/GaussFitter.h>
///////////////////////////

using namespace OpenMS;
using namespace Math;
using namespace std;

START_TEST(GaussFitter, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

GaussFitter* ptr = 0;
GaussFitter* nullPointer = 0;
START_SECTION(GaussFitter())
{
	ptr = new GaussFitter();
	TEST_NOT_EQUAL(ptr, nullPointer)
}
END_SECTION

START_SECTION((virtual ~GaussFitter()))
{
  delete ptr;
	NOT_TESTABLE
}
END_SECTION

START_SECTION((GaussFitResult fit(std::vector< DPosition< 2 > >& points)))
{
  DPosition<2> pos;
	pos.setX(0.0);
	pos.setY(0.01);
	vector<DPosition<2> > points;
	points.push_back(pos);
	pos.setX(0.05);
	pos.setY(0.2);
	points.push_back(pos);
	pos.setX(0.16);
	pos.setY(0.63);
	points.push_back(pos);
	pos.setX(0.28);
	pos.setY(0.99);
	points.push_back(pos);
	pos.setX(0.66);
	pos.setY(0.03);
	points.push_back(pos);
	pos.setX(0.50);
	pos.setY(0.36);
	points.push_back(pos);
	
	ptr = new GaussFitter;
	GaussFitter::GaussFitResult result = ptr->fit(points);

	TOLERANCE_ABSOLUTE(0.1)
	TEST_REAL_SIMILAR(result.A, 1.0)
	TEST_REAL_SIMILAR(result.x0, 0.3)
	TEST_REAL_SIMILAR(result.sigma, 0.2)
}
END_SECTION

START_SECTION((void setInitialParameters(const GaussFitResult& result)))
{
  GaussFitter f1;
  GaussFitter::GaussFitResult result (-1,-1,-1);
  f1.setInitialParameters(result);

	NOT_TESTABLE //implicitly tested in fit method
}
END_SECTION


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST



