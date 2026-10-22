#ifndef RENDERATTACHMENT_H
#define RENDERATTACHMENT_H


#include "resource.h"
#include "../Platform/vulkanPlatform.h"
 
namespace Centauri {

    
    class RenderAttachment : public Resource {
        public:
            explicit RenderAttachment(const std::string& id, VulkanPlatform* plat, int w, int h, vk::ImageUsageFlags usg, vk::Format form,
                            vk::ImageAspectFlags aspect, vk::SampleCountFlagBits msaa = vk::SampleCountFlagBits::e1) : 
                            Resource(id), vkPlat(plat), usage(usg), width(w), height(h),
                            format(form), aspectFlags(aspect), msaaSamples(msaa) {}

            ~RenderAttachment() override {
                Unload();
            }

            bool Load() override;
            void Unload() override;

            vk::raii::Image& GetImage() {return image;}
            vk::raii::ImageView& GetImageView() {return imageView;}

        private:
            vk::raii::Image image = nullptr;
            vk::raii::DeviceMemory memory = nullptr;
            vk::DeviceSize offset;
            vk::raii::ImageView imageView = nullptr;
            vk::Format format;
            vk::ImageAspectFlags aspectFlags;

            int width = 0;
            int height = 0;
            int channels = 0;
            vk::SampleCountFlagBits msaaSamples = vk::SampleCountFlagBits::e1;

            VulkanPlatform* vkPlat = nullptr;
            vk::ImageUsageFlags usage;
        
            
    };
}     

#endif //RENDERATTACHMENT_H
