const postProcessing = require('./post_processing')
const preProcessing = require('./pre_processing')

module.exports = function(inferenceEngine) {
    inferenceEngine.postProcessing = postProcessing
    inferenceEngine.preProcessing = preProcessing
    return inferenceEngine
}
