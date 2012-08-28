// @HEADER
// ***********************************************************************
//
//           Panzer: A partial differential equation assembly
//       engine for strongly coupled complex multiphysics systems
//                 Copyright (2011) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Roger P. Pawlowski (rppawlo@sandia.gov) and
// Eric C. Cyr (eccyr@sandia.gov)
// ***********************************************************************
// @HEADER

#ifndef PANZER_MODEL_EVALUATOR_EPETRA_HPP
#define PANZER_MODEL_EVALUATOR_EPETRA_HPP

#include "EpetraExt_ModelEvaluator.h"

#include "Epetra_Map.h"
#include "Epetra_Vector.h"
#include "Epetra_Comm.h"
#include "Epetra_CrsGraph.h"

#include "Teuchos_RCP.hpp"
#include "Teuchos_AbstractFactory.hpp"

#include "Panzer_Traits.hpp"
#include "Panzer_AssemblyEngine_TemplateManager.hpp"
#include "Panzer_ParameterLibrary.hpp"

#include "Thyra_VectorBase.hpp"

#include <boost/tuple/tuple.hpp>
#include <vector>
#include <string>

namespace panzer {

  template<typename, typename>  class FieldManagerBuilder;
  template<typename, typename>  class EpetraLinearObjFactory;
  #ifdef HAVE_STOKHOS
     template<typename, typename>  class SGEpetraLinearObjFactory;
  #endif
  template<typename>  class ResponseLibrary;
  class EpetraLinearObjContainer;
  class SGEpetraLinearObjContainer;
  class GlobalData;

  class ModelEvaluator_Epetra : public EpetraExt::ModelEvaluator {
  public:

    ModelEvaluator_Epetra(const Teuchos::RCP<panzer::FieldManagerBuilder<int,int> >& fmb,
                          const Teuchos::RCP<panzer::ResponseLibrary<panzer::Traits> >& rLibrary,
			  const Teuchos::RCP<panzer::LinearObjFactory<panzer::Traits> >& lof,
			  const std::vector<Teuchos::RCP<Teuchos::Array<std::string> > >& p_names,
			  const Teuchos::RCP<panzer::GlobalData>& global_data,
			  bool build_transient_support);
    
    ModelEvaluator_Epetra(const Teuchos::RCP<panzer::FieldManagerBuilder<int,int> >& fmb,
                          const Teuchos::RCP<panzer::ResponseLibrary<panzer::Traits> >& rLibrary,
			  const Teuchos::RCP<panzer::EpetraLinearObjFactory<panzer::Traits,int> >& lof,
			  const std::vector<Teuchos::RCP<Teuchos::Array<std::string> > >& p_names,
			  const Teuchos::RCP<panzer::GlobalData>& global_data,
			  bool build_transient_support);

    #ifdef HAVE_STOKHOS
       ModelEvaluator_Epetra(const Teuchos::RCP<panzer::FieldManagerBuilder<int,int> >& fmb,
                             const Teuchos::RCP<panzer::ResponseLibrary<panzer::Traits> >& rLibrary,
   			     const Teuchos::RCP<panzer::SGEpetraLinearObjFactory<panzer::Traits,int> >& sg_lof,
   			     const std::vector<Teuchos::RCP<Teuchos::Array<std::string> > >& p_names,
			     const Teuchos::RCP<panzer::GlobalData>& global_data,
			     bool build_transient_support);
    #endif
    
    /** \name Overridden from EpetraExt::ModelEvaluator . */
    //@{
    
    Teuchos::RCP<const Epetra_Map> get_x_map() const;
    Teuchos::RCP<const Epetra_Map> get_f_map() const;
    Teuchos::RCP<const Epetra_Vector> get_x_init() const;
    Teuchos::RCP<const Epetra_Vector> get_x_dot_init() const;
    double get_t_init() const;
    Teuchos::RCP<Epetra_Operator> create_W() const;
    Teuchos::RCP<const Epetra_Map> get_p_map(int l) const;
    Teuchos::RCP<const Teuchos::Array<std::string> > get_p_names(int l) const;
    Teuchos::RCP<const Epetra_Vector> get_p_init(int l) const;
    Teuchos::RCP<const Epetra_Map> get_g_map(int l) const;
    InArgs createInArgs() const;
    OutArgs createOutArgs() const;
    void evalModel( const InArgs& inArgs, const OutArgs& outArgs ) const;

    //@}

    /** \brief Set initial time value */
    void set_t_init(double t);

    //! Get the response library used by this evaluator
    Teuchos::RCP<panzer::ResponseLibrary<panzer::Traits> > getResponseLibrary() const
    { return responseLibrary_; }

    /** \name Post-Construction methods to add parameters and/or responses */
    //@{
    
    /** Add a distributed parameter to the model evaluator

        Distributed parameters are special in that they most likely
        will require a global to ghost call before being used in the
        evaluator.  This function registers the parameter and any
        needed machinery to perform the global to ghost call.

        NOTE: We can't use the LinearObjFactory and
        LinearObjContainers here because those objects require a
        unique global indexer to build.  In general, the distributed
        parameters may NOT be coming from an object that has an
        associated unique global indexer.  An example of this is
        multiphysics coupling.  The parameters will be coming form
        another code that may not have a PDE discretization.
        Generalizing this function to hide the linear algebra type may
        not be possible unless we refactor the linear object support
        or write new wrapper objects.  Also note that Thyra has no
        concept of an import/export object so we can't use Thyra here
        to abstract the objects.

        \param[in] name Name of the distributed parameter
	\param[in] global_map RCP to Epetra_Map used to construct the global parameter vector.
	\param[in] importer RCP to a Epetra_Import object used for the global to ghost.  If set to null, then no global to ghost will be performed.  
	\param[in] ghosted_vector RCP to the ghosted vector that is the target of the global to ghost.  If set to null, then no global to ghost will be performed.  

	\return The index associated with this parameter for accessing it through the ModelEvaluator interface.
    */
    int addDistributedParameter(const std::string name,
				const Teuchos::RCP<Epetra_Map>& global_map,
				const Teuchos::RCP<Epetra_Import>& importer,
				const Teuchos::RCP<Epetra_Vector>& ghosted_vector);

    //@}

  private:

    // /////////////////////////////////////
    // Private methods

    /** Initialize Epetra linear objects.
      *
      * \note Requires lof_ to be set.
      */
    void initializeEpetraObjs(panzer::EpetraLinearObjFactory<panzer::Traits,int> & lof);

    /** Initialize the parameter vector object */
    void initializeParameterVector(const std::vector<Teuchos::RCP<Teuchos::Array<std::string> > >& p_names,
				   const Teuchos::RCP<panzer::ParamLib>& parameter_library);

    // /////////////////////////////////////
    // Private evaluation methods

    //! for evaluation and handling of normal quantities, x,f,W, etc
    void evalModel_basic( const InArgs& inArgs, const OutArgs& outArgs ) const; 

    /** handles evaluation of responses g, dgdx
      *
      * \note This method should (basically) be a no-op if <code>required_basic_g(outArgs)==false</code>.
      *       However, for efficiency this is not checked.
      */
    void evalModel_basic_g(AssemblyEngineInArgs ae_inargs,const InArgs & inArgs,const OutArgs & outArgs) const;

    //! Are their required responses in the out args? g (and soon DgDx) 
    bool required_basic_g(const OutArgs & outArgs) const;

    #ifdef HAVE_STOKHOS
       //! Are their required SG responses in the out args? sg
       bool required_basic_sg_g(const OutArgs & outArgs) const;

       /** for evaluation and handling of Stochastic Galerkin quantities, x_sg, f_sg, W_sg, etc
         *
         * \note A precondition for this is that <code>sg_lof_</code> has been initialized
         *       with a call to the appropriate constructor.
         */
       void evalModel_sg( const InArgs& inArgs, const OutArgs& outArgs ) const;

       //! Are their required responses in the out args? g (and soon DgDx) 
       bool required_sg_g(const OutArgs & outArgs) const;

       /** handles evaluation of responses g, dgdx
         *
         * \note This method should (basically) be a no-op if <code>required_basic_g(outArgs)==false</code>.
         *       However, for efficiency this is not checked.
         */
       void evalModel_sg_g(AssemblyEngineInArgs ae_inargs,const InArgs & inArgs,const OutArgs & outArgs) const;
    #endif

    void copyEpetraIntoThyra(const Epetra_MultiVector& x, const Teuchos::Ptr<Thyra::VectorBase<double> > &thyraVec) const;
    void copyThyraIntoEpetra(const Thyra::VectorBase<double>& thyraVec, Epetra_MultiVector& x) const;

    // /////////////////////////////////////
    // Private member data
    
    /** \defgroup EpetraObjs Underlying epetra types
      * @{ 
      */
    Teuchos::RCP<const Epetra_Map>   map_x_;
    Teuchos::RCP<Epetra_Vector> x0_;
    Teuchos::RCP<Epetra_Vector> x_dot_init_;
    double t_init_;
    mutable Teuchos::RCP<Epetra_Vector> dummy_f_;    
    
    // parameters
    std::vector<Teuchos::RCP<Epetra_Map> > p_map_;
    std::vector<Teuchos::RCP<Epetra_Vector> > p_init_;

    // responses
    std::vector<Teuchos::RCP<Epetra_Map> > g_map_;
    
    /** @} */
    
    Teuchos::RCP<panzer::FieldManagerBuilder<int,int> > fmb_;
    mutable Teuchos::RCP<panzer::ResponseLibrary<panzer::Traits> > responseLibrary_; // These objects are basically the same
    mutable panzer::AssemblyEngine_TemplateManager<panzer::Traits,int,int> ae_tm_;   // they control and provide access to evaluate
    std::vector<Teuchos::RCP<Teuchos::Array<std::string> > > p_names_;
    //Teuchos::RCP<panzer::ParamLib> parameter_library_;
    mutable Teuchos::Array<panzer::ParamVec> parameter_vector_;
    Teuchos::RCP<panzer::GlobalData> global_data_;
    bool build_transient_support_;

    /** Returns true if this is a distributed vector and false if it is a locally replicated scalar parameter.  This is used to determine when to call a global to ghost method which is required for distributed parameters only.
    */
    std::vector<bool> is_distributed_parameter_;

    /** Vector of Boost tuples that contains objects needed for the global to ghost method for distributed parameters.

       Tuple index 0: the string name for the parameter in the model evaluator.
       Tuple index 1: the integer index for the parameter in the model evaluator.
       Tuple index 2: an RCP to the linear object factory that performs the global to ghost operation.
       Tuple index 3: an RCP to the GHOSTED vector that is the target of the global to ghost operation. 
    */ 
    std::vector<boost::tuple<std::string,int,Teuchos::RCP<Epetra_Import>,Teuchos::RCP<Epetra_Vector> > > distributed_parameter_container_;

    // basic specific linear object objects
    Teuchos::RCP<panzer::LinearObjFactory<panzer::Traits> > lof_;
    mutable Teuchos::RCP<LinearObjContainer> ghostedContainer_;

    #ifdef HAVE_STOKHOS
       // sg specific linear object objects
       Teuchos::RCP<panzer::SGEpetraLinearObjFactory<panzer::Traits,int> > sg_lof_;
       mutable Teuchos::RCP<LinearObjContainer> sg_ghostedContainer_;
    #endif

    Teuchos::RCP<Teuchos::AbstractFactory<Epetra_Operator> > epetraOperatorFactory_;
  };

  /** From a genericly typed linear object factory try and build an epetra model evaluator.
    * This method attempts to cast to the right linear object factory and then calls the
    * appropriate constructor of ModelEvaluator_Epetra.
    */
  Teuchos::RCP<ModelEvaluator_Epetra> 
  buildEpetraME(const Teuchos::RCP<FieldManagerBuilder<int,int> >& fmb,
                const Teuchos::RCP<ResponseLibrary<panzer::Traits> >& rLibrary,
	        const Teuchos::RCP<LinearObjFactory<panzer::Traits> >& lof,
	        const std::vector<Teuchos::RCP<Teuchos::Array<std::string> > >& p_names,
		const Teuchos::RCP<panzer::GlobalData>& global_data,
	        bool build_transient_support);
  
}

#endif
