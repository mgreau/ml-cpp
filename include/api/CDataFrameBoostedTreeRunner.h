/*
 * Copyright Elasticsearch B.V. and/or licensed to Elasticsearch B.V. under one
 * or more contributor license agreements. Licensed under the Elastic License;
 * you may not use this file except in compliance with the Elastic License.
 */

#ifndef INCLUDED_ml_api_CDataFrameBoostedTreeRunner_h
#define INCLUDED_ml_api_CDataFrameBoostedTreeRunner_h

#include <api/CDataFrameAnalysisRunner.h>

#include <api/ImportExport.h>

#include <rapidjson/fwd.h>

namespace ml {
namespace maths {
class CBoostedTree;
}
namespace api {

//! \brief Runs boosted tree regression on a core::CDataFrame.
class API_EXPORT CDataFrameBoostedTreeRunner final : public CDataFrameAnalysisRunner {
public:
    //! This is not intended to be called directly: use CDataFrameBoostedTreeRunnerFactory.
    CDataFrameBoostedTreeRunner(const CDataFrameAnalysisSpecification& spec,
                                const rapidjson::Value& jsonParameters);

    //! This is not intended to be called directly: use CDataFrameBoostedTreeRunnerFactory.
    CDataFrameBoostedTreeRunner(const CDataFrameAnalysisSpecification& spec);

    ~CDataFrameBoostedTreeRunner();

    //! \return The number of columns this adds to the data frame.
    std::size_t numberExtraColumns() const override;

    //! Write the prediction for \p row to \p writer.
    void writeOneRow(const TStrVec& featureNames,
                     TRowRef row,
                     core::CRapidJsonConcurrentLineWriter& writer) const override;

private:
    using TBoostedTreeUPtr = std::unique_ptr<maths::CBoostedTree>;

private:
    void runImpl(core::CDataFrame& frame) override;
    std::size_t estimateBookkeepingMemoryUsage(std::size_t numberPartitions,
                                               std::size_t totalNumberRows,
                                               std::size_t partitionNumberRows,
                                               std::size_t numberColumns) const override;

private:
    // Note custom config is written directly to the boosted tree object.

    //! The tree.
    TBoostedTreeUPtr m_BoostedTree;
};

//! \brief Makes a core::CDataFrame boosted tree regression runner.
class API_EXPORT CDataFrameBoostedTreeRunnerFactory final : public CDataFrameAnalysisRunnerFactory {
public:
    const char* name() const override;

private:
    TRunnerUPtr makeImpl(const CDataFrameAnalysisSpecification& spec) const override;
    TRunnerUPtr makeImpl(const CDataFrameAnalysisSpecification& spec,
                         const rapidjson::Value& params) const override;
};
}
}

#endif // INCLUDED_ml_api_CDataFrameBoostedTreeRunner_h
