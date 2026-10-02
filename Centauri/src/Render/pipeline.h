#ifndef PIPELINE_H
#define PIPELINE_H

namespace Centauri {
    class Pipeline {
      public:
        virtual ~Pipeline() = default;
        
        static Pipeline* CreatePipeline();

      private:
        
    };
}

#endif //PIPELINE_H