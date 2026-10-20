#ifndef RENDERATTACHMENT_H
#define RENDERATTACHMENT_H


#include "resource.h"
#include "../Platform/vulkanPlatform.h"
 
namespace Centauri {

    
    class RenderAttachment : public Resource {
        public:
            explicit RenderAttachment(const std::string& id, VulkanPlatform* plat, int w, int h, vk::ImageUsageFlags usg, vk::Format form,
                            vk::SampleCountFlagBits msaa = vk::SampleCountFlagBits::e1) : 
                            Resource(id), vkPlat(plat), usage(usg), width(w), height(h),
                            format(form), msaaSamples(msaa) {}

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
            vk::Format format;

            int width = 0;
            int height = 0;
            int channels = 0;
            vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;

            VulkanPlatform* vkPlat = nullptr;
            vk::ImageUsageFlags usage;
        
            
    };
}     

#endif //RENDERATTACHMENT_H
