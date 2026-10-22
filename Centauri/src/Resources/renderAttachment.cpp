#include "renderAttachment.h"

namespace Centauri {
    bool RenderAttachment::Load() {
        
        std::tie(image, memory) = vkPlat -> CreateImage(width, height, 1, msaaSamples, format, 
															vk::ImageTiling::eOptimal,
															usage, 
															vk::MemoryPropertyFlagBits::eDeviceLocal);
        imageView = std::move(vkPlat -> CreateImageView(image, format, aspectFlags, 1));	
        return true;
    }

    void RenderAttachment::Unload() {
        //Should we unload memory???
    }
   
    
}