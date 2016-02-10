#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "mblas/base_matrix.h"

class Weights;
class Vocab;
class Encoder;
class Decoder;

struct WhichState {
  WhichState()
  : stateId(0), rowNo(0) {}
  
  WhichState(size_t s, size_t r)
  : stateId(s), rowNo(r) {}
  
  size_t stateId;
  size_t rowNo;
};

class NMT {
  public:
    NMT(const boost::shared_ptr<Weights> model,
        const boost::shared_ptr<Vocab> src,
        const boost::shared_ptr<Vocab> trg);
  
    static size_t GetDevices();
    void SetDevice();
  
    static boost::shared_ptr<Weights> NewModel(const std::string& path, size_t device = 0);
  
    static boost::shared_ptr<Vocab> NewVocab(const std::string& path);
  
    void CalcSourceContext(const std::vector<std::string>& s);
    
    void MakeStep(
      const std::vector<std::string>& nextWords,
      const std::vector<std::string>& lastWords,
      std::vector<WhichState>& inputStates,
      std::vector<double>& logProbs,
      std::vector<WhichState>& nextStates,
      std::vector<bool>& unks);
  
    void ClearStates();
    
  private:
    const boost::shared_ptr<Weights> w_;
    const boost::shared_ptr<Vocab> src_;
    const boost::shared_ptr<Vocab> trg_;
    
    boost::shared_ptr<Encoder> encoder_;
    boost::shared_ptr<Decoder> decoder_;
    
    boost::shared_ptr<mblas::BaseMatrix> SourceContext;
    
    std::vector<boost::shared_ptr<mblas::BaseMatrix> > states_;
};