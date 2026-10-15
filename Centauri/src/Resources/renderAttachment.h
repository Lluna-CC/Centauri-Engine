#ifndef IMAGE_H
#define IMAGE_H


#include "resource.h"
#include "../Platform/vulkanPlatform.h"
 
namespace Centauri {

    
    class RenderAttachment : public Resource {
        public:
            enum AttachmentType {
                Depth,
                Color
            };

            explicit RenderAttachment(const std::string& id, VulkanPlatform* plat, AttachmentType type,
                            vk::SampleCountFlagBits msaa = vk::SampleCountFlagBits::e1) : Resource(id), vkPlat(plat), attType(type), msaaSamples(msaa) {}

            ~RenderAttachment() override {
                Unload();
            }

            bool Load() override;
            void Unload() override;

            vk::Image GetImage() const {return image;}
            vk::ImageView GetImageView() const {return imageView;}

        private:
            vk::Image image;
            vk::DeviceMemory memory;
            vk::DeviceSize offset;
            vk::ImageView imageView;

            int width = 0;
            int height = 0;
            int channels = 0;
            vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;

            VulkanPlatform* vkPlat = nullptr;
            AttachmentType attType;
        
            
    };
}     

#endif //IMAGE_H 
