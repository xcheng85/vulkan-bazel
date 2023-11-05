#define VK_NO_PROTOTYPES // volk
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <fstream>
#include <format>

#include <boost/di.hpp>
#include <spdlog/spdlog.h>

#include "engine/core/application.h"
#include "engine/core/window.h"
#include "engine/core/platform.h"

namespace di = boost::di;

using namespace std;
using namespace Engine::Core;
using namespace spdlog;

class VulkanApplication : public Application
{
public:
    VulkanApplication(
        // std::unique_ptr<IContext> ctx
        )
        : Application(
              // std::move(ctx)
          )
    {
        info("--> VulkanApplication::VulkanApplication");
        // initSync();
        // initCommandBuffers();
        // initRenderPass();
        // initPipeline();
        // initFramebuffer();
        // recordCommandBuffers();
        info("<-- VulkanApplication::VulkanApplication");
    };

    virtual ~VulkanApplication()
    {
    }

    void update() override
    {
        render();
    }
    virtual void render()
    {
        prepare_frame();
        draw();
        submit_frame();
    }

    void resize(const uint32_t width, const uint32_t height) override
    {
        // auto vkDevice = device->getVkHandle();
        // vkDeviceWaitIdle(vkDevice);
        // destroyFramebuffer();
        // destroyCommandBuffers();
        // destroySync();
        // // let context take resize handler first, swap chain will be recreated
        // // old swap chain and image view is clearned by smart unique_ptr
        // Application::resize(width, height);

        // this->initSync();
        // this->initCommandBuffers();
        // this->initFramebuffer();
        // this->recordCommandBuffers();
    }

protected:
    // shareable code for acqure image from swap chain
    void prepare_frame()
    {
        // // Acquire the next image from the swap chain
        // auto vkSwapChain = dynamic_cast<VulkanSwapChain *>(this->_context->getSwapChain());
        // VkResult result = vkSwapChain->acquireNextImage(current_image_index, acquired_image_ready, VK_NULL_HANDLE);
        // // Recreate the swapchain if it's no longer compatible with the surface (OUT_OF_DATE)
        // if (result == VK_ERROR_OUT_OF_DATE_KHR)
        // {
        //     cout << "VK_ERROR_OUT_OF_DATE_KHR" << endl;
        // }
        // else if (result != VK_SUBOPTIMAL_KHR)
        // {
        //     VK_CHECK(result);
        // }
    }
    // shareable code for present rendering to the image
    void submit_frame()
    {
        // if (this->queue != VK_NULL_HANDLE)
        // {
        //     auto vkSwapChain = this->_context->getSwapChain()->getVkHandle();

        //     // present current_image_index of swapchain images
        //     VkPresentInfoKHR present_info = {
        //         .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        //         .pNext = NULL,
        //         .swapchainCount = 1,
        //         .pSwapchains = &vkSwapChain,
        //         .pImageIndices = &current_image_index,
        //     };

        //     if (render_complete != VK_NULL_HANDLE)
        //     {
        //         present_info.pWaitSemaphores = &render_complete;
        //         present_info.waitSemaphoreCount = 1;
        //     }

        //     VkResult present_result = vkQueuePresentKHR(queue, &present_info);
        //     if (!((present_result == VK_SUCCESS) || (present_result == VK_SUBOPTIMAL_KHR)))
        //     {
        //         if (present_result == VK_ERROR_OUT_OF_DATE_KHR)
        //         {
        //             // Swap chain is no longer compatible with the surface and needs to be recreated
        //             // resize(width, height);
        //             return;
        //         }
        //         else
        //         {
        //             VK_CHECK(present_result);
        //         }
        //     }
        //}

        // // DO NOT USE
        // // vkDeviceWaitIdle and vkQueueWaitIdle are extremely expensive functions, and are used here purely for demonstrating the vulkan API
        // // without having to concern ourselves with proper syncronization. These functions should NEVER be used inside the render loop like this (every frame).
        // VK_CHECK(vkDeviceQueue.wait_idle());
    }

    virtual void draw(
        // CommandBuffer &command_buffer, RenderTarget &render_target
    )
    {
        // //  used in the call to vkQueueSubmit is unrecorded and contains no commands
        // // must recorded
        // submit_info.commandBufferCount = 1;
        // submit_info.pCommandBuffers = &commandBuffers[current_image_index];
        // VK_CHECK(vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE));
    }

    void initSync()
    {
        // // device and host sync mechanisum
        // // do i need semaphore pool ?

        // // semaphore: queue level
        // // fence: command buffer level-> between command buffers
        // // events: within 1 command buffer

        // // fence to guard in the host app level
        // // fence reuse

        // VkSemaphoreCreateInfo semaphore_create_info{.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        // this->device = dynamic_cast<VulkanLogicalDevice *>(_context->getLogicDevice());
        // auto vkDevice = device->getVkHandle();
        // auto vkSwapChain = dynamic_cast<VulkanSwapChain *>(this->_context->getSwapChain());

        // VK_CHECK(vkCreateSemaphore(vkDevice, &semaphore_create_info, nullptr, &acquired_image_ready));
        // VK_CHECK(vkCreateSemaphore(vkDevice, &semaphore_create_info, nullptr, &render_complete));
    }

    void initRenderPass()
    {
        // auto vkDevice = device->getVkHandle();
        // auto vkSwapChain = dynamic_cast<VulkanSwapChain *>(this->_context->getSwapChain());
        // // render pass
        // // all the attachments in the render pass
        // // use the same format as images of swap chains
        // auto colorFormat = vkSwapChain->getSurfaceFormat().format;
        // // all attachment used in the render pass
        // // here we only need color
        // VkAttachmentDescription colorAttachment = {
        //     .flags = 0,
        //     .format = colorFormat,
        //     .samples = VK_SAMPLE_COUNT_1_BIT,
        //     .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        //     .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        //     // Don't care about stencil since we're not using it.
        //     .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        //     .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        //     // offscreen: VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        //     // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        //     .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        //     // last pass: VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, src of swap chain present
        //     // not lasspass: VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        //     .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        // };
        // // subpasses: operations in a render pass
        // // minimum 1 subpass
        // // each subpass will interact with attachment, and the layout transition
        // // color, depth/stencil, resolve(multisampling), preserve
        // VkAttachmentReference colorAttachmentRef = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
        // VkSubpassDescription subpass = {
        //     .flags = 0,
        //     .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        //     .inputAttachmentCount = 0,
        //     .pInputAttachments = nullptr,
        //     .colorAttachmentCount = 1, // write into
        //     .pColorAttachments = &colorAttachmentRef,
        //     .pResolveAttachments = nullptr,
        //     .pDepthStencilAttachment = nullptr,
        //     .preserveAttachmentCount = 0,
        //     .pPreserveAttachments = nullptr};
        // // subpass dependencies
        // // VK_SUBPASS_EXTERNAL: all the commands before the render pass
        // // producing subpass: VK_SUBPASS_EXTERNAL,
        // // consuming subpass: subpass 0, we have only 1 here.
        // // VK_SUBPASS_EXTERNAL must finish first.
        // VkSubpassDependency dependency = {
        //     .srcSubpass = VK_SUBPASS_EXTERNAL,
        //     .dstSubpass = 0,
        //     .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        //     .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        //     .srcAccessMask = 0,
        //     .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        // };

        // // finally, renderPass
        // VkRenderPassCreateInfo renderPassInfo = {
        //     .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        //     .pNext = nullptr,
        //     .flags = 0,
        //     .attachmentCount = 1,
        //     .pAttachments = &colorAttachment,
        //     .subpassCount = 1,
        //     .pSubpasses = &subpass,
        //     .dependencyCount = 1,
        //     .pDependencies = &dependency};
        // VK_CHECK(vkCreateRenderPass(vkDevice, &renderPassInfo, nullptr, &render_pass));
    }

    void initCommandBuffers()
    {
        // // command pool
        // // each threads should create its own command pool
        // // queue family index
        // // two design:
        // // 1. each swapchain image has command pool and create 1 command buffer (order random, needs fence in the host app)
        // // 2. each swapchain image create different command buffer from the same command pool (order is respected)
        // const auto &allPresentableQueues = device->getDeviceQueues();
        // auto vkDevice = device->getVkHandle();
        // auto vkSwapChain = dynamic_cast<VulkanSwapChain *>(this->_context->getSwapChain());
        // if (allPresentableQueues.size() > 0)
        // {
        //     this->queue = allPresentableQueues[0]->getVKQueue();
        //     const auto &familyIndex = allPresentableQueues[0]->getFamilyIndex();

        //     // three pool and three command buffers
        //     VkCommandPoolCreateInfo command_pool_info = {
        //         .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        //         .pNext = nullptr,
        //         .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, /* Allow command from this pool buffers to be reset*/
        //         .queueFamilyIndex = familyIndex,
        //     };

        //     VK_CHECK(vkCreateCommandPool(vkDevice, &command_pool_info, nullptr, &cmd_pool));

        //     commandBuffers.resize(vkSwapChain->getImages().size());
        //     const VkCommandBufferAllocateInfo command_buffer_ai = {
        //         .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        //         .pNext = nullptr,
        //         .commandPool = cmd_pool,
        //         .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        //         .commandBufferCount = commandBuffers.size(),
        //     };
        //     VK_CHECK(vkAllocateCommandBuffers(vkDevice, &command_buffer_ai, commandBuffers.data()));
        // }
        // else
        // {
        //     throw std::runtime_error("Presentable Graphics Queue not found");
        // }
    }

    void initPipeline()
    {
        // auto vkDevice = device->getVkHandle();
        // // shader module and pipeline
        // // opengl: state machine
        // // vulkan: all the shader variation means a different pipline obj
        // auto readFile = [](const std::string &filename)
        // {
        //     std::ifstream file(filename, std::ios::ate | std::ios::binary);

        //     if (!file.is_open())
        //     {
        //         throw std::runtime_error("failed to open file!");
        //     }

        //     size_t fileSize = (size_t)file.tellg();
        //     std::vector<unsigned int> buffer(fileSize / 4);

        //     file.seekg(0);
        //     file.read(reinterpret_cast<char *>(buffer.data()), fileSize);
        //     file.close();

        //     return buffer;
        // };
        // ShaderModule vertShaderModule;
        // ShaderModule fragShaderModule;
        // vertShaderModule.SPIRV = readFile("../data/shaders/full.vert.spv");
        // fragShaderModule.SPIRV = readFile("../data/shaders/full.frag.spv");
        // const VkShaderModuleCreateInfo vertShaderCreateInfo =
        //     {
        //         .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        //         .codeSize = vertShaderModule.SPIRV.size() * sizeof(unsigned int),
        //         .pCode = vertShaderModule.SPIRV.data(),
        //     };
        // const VkShaderModuleCreateInfo fragShaderCreateInfo =
        //     {
        //         .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        //         .codeSize = fragShaderModule.SPIRV.size() * sizeof(unsigned int),
        //         .pCode = fragShaderModule.SPIRV.data(),
        //     };
        // VK_CHECK(vkCreateShaderModule(vkDevice, &vertShaderCreateInfo, nullptr, &vertShaderModule.shaderModule));
        // VK_CHECK(vkCreateShaderModule(vkDevice, &fragShaderCreateInfo, nullptr, &fragShaderModule.shaderModule));

        // // graphics rendering pipeline
        // // 1. pipeline layout: allow pipeline to access arrays of descriptor sets and push constants
        // // 2. dynamic state: state that could be changed in the runtime.
        // // 3. vertex input state: input binding and input attribute
        // // 4. vertex input assembly: primitive restart (useful for strips)
        // // 5. rasterization state
        // // 6. blend state
        // // 7. viewport
        // // 8. depth and stencil test
        // // 9. multi-sampling
        // // 10. shader stages
        // // Specify that these states will be dynamic, i.e. not part of pipeline state object.
        // std::array<VkDynamicState, 2> dynamics{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        // VkPipelineDynamicStateCreateInfo dynamic{
        //     .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        //     .dynamicStateCount = dynamics.size(),
        //     .pDynamicStates = dynamics.data(),
        // };
        // // here we do it in the vs directly. skip the input binding and attribute
        // VkPipelineVertexInputStateCreateInfo vertex_input{
        //     .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        // };
        // VkPipelineInputAssemblyStateCreateInfo input_assembly{
        //     .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        //     .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        // };

        // VkPipelineRasterizationStateCreateInfo raster{
        //     .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        //     .cullMode = VK_CULL_MODE_BACK_BIT,
        //     .frontFace = VK_FRONT_FACE_CLOCKWISE,
        //     .lineWidth = 1.0f,
        // };

        // VkPipelineColorBlendAttachmentState blend_attachment{
        //     .blendEnable = false,
        //     .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};

        // VkPipelineColorBlendStateCreateInfo blend{
        //     .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        //     .attachmentCount = 1,
        //     .pAttachments = &blend_attachment,
        // };

        // VkPipelineViewportStateCreateInfo viewport{
        //     .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        //     .viewportCount = 1,
        //     .scissorCount = 1,
        // };

        // // Disable all depth testing.
        // VkPipelineDepthStencilStateCreateInfo depth_stencil{
        //     .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        //     .depthTestEnable = false,
        //     .stencilTestEnable = false,
        // };

        // VkPipelineMultisampleStateCreateInfo multisample{
        //     .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        //     .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        // };

        // VkPipelineLayoutCreateInfo layout_info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        // VK_CHECK(vkCreatePipelineLayout(vkDevice, &layout_info, nullptr, &pipelineLayout));

        // // Load our SPIR-V shaders.
        // std::array<VkPipelineShaderStageCreateInfo, 2> shader_stages{};

        // // what programmable stages are active when the pipeline is bounded to the command buffer
        // shader_stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        // shader_stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        // shader_stages[0].module = vertShaderModule.shaderModule;
        // shader_stages[0].pName = "main";

        // shader_stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        // shader_stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        // shader_stages[1].module = fragShaderModule.shaderModule;
        // shader_stages[1].pName = "main";

        // // ready to create pipeline
        // const VkGraphicsPipelineCreateInfo pipelineInfo = {
        //     .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        //     .stageCount = static_cast<uint32_t>(shader_stages.size()),
        //     .pStages = shader_stages.data(),
        //     .pVertexInputState = &vertex_input,
        //     .pInputAssemblyState = &input_assembly,
        //     .pTessellationState = nullptr,
        //     .pViewportState = &viewport,
        //     .pRasterizationState = &raster,
        //     .pMultisampleState = &multisample,
        //     .pDepthStencilState = &depth_stencil,
        //     .pColorBlendState = &blend,
        //     .pDynamicState = &dynamic,
        //     .layout = pipelineLayout,
        //     .renderPass = render_pass,
        // };
        // VK_CHECK(vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline));

        // // Pipeline is baked, we can delete the shader modules now.
        // vkDestroyShaderModule(vkDevice, shader_stages[0].module, nullptr);
        // vkDestroyShaderModule(vkDevice, shader_stages[1].module, nullptr);
    }
    // framebuffer are used together with render pass
    // framebuffer specify image resource to be used as the attachments in the renderpass
    // size of render area
    void initFramebuffer()
    {
        // auto vkDevice = device->getVkHandle();
        // auto vkSwapChain = dynamic_cast<VulkanSwapChain *>(this->_context->getSwapChain());
        // const auto &extents = vkSwapChain->getImageExtents();

        // for (auto &image_view : vkSwapChain->getImageViews())
        // {
        //     // Build the framebuffer.
        //     VkFramebufferCreateInfo fb_info{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
        //     fb_info.renderPass = render_pass;
        //     // bind swapchain imageview to the renderpass's attachment
        //     fb_info.attachmentCount = 1;
        //     fb_info.pAttachments = &image_view;
        //     fb_info.width = extents.width;
        //     fb_info.height = extents.height;
        //     fb_info.layers = 1;

        //     VkFramebuffer framebuffer;
        //     VK_CHECK(vkCreateFramebuffer(vkDevice, &fb_info, nullptr, &framebuffer));

        //     swapchain_framebuffers.push_back(framebuffer);
        // }
    }

    void destroySync()
    {
        // auto vkDevice = device->getVkHandle();
        // vkDestroySemaphore(vkDevice, this->acquired_image_ready, nullptr);
        // vkDestroySemaphore(vkDevice, this->render_complete, nullptr);
    }

    void destroyCommandBuffers()
    {
        // auto vkDevice = device->getVkHandle();
        // vkFreeCommandBuffers(vkDevice, cmd_pool, commandBuffers.size(), commandBuffers.data());
        // vkDestroyCommandPool(vkDevice, cmd_pool, nullptr);
    }

    void destroyRenderPass()
    {
        // auto vkDevice = device->getVkHandle();
        // vkDestroyRenderPass(vkDevice, render_pass, nullptr);
    }

    void destroyPipeline()
    {
        // auto vkDevice = device->getVkHandle();
        // vkDestroyPipeline(vkDevice, this->pipeline, nullptr);
        // vkDestroyPipelineLayout(vkDevice, this->pipelineLayout, nullptr);
    }

    void destroyFramebuffer()
    {
        // auto vkDevice = device->getVkHandle();
        // vkQueueWaitIdle(this->queue);

        // for (auto &framebuffer : this->swapchain_framebuffers)
        // {
        //     vkDestroyFramebuffer(vkDevice, framebuffer, nullptr);
        // }

        // this->swapchain_framebuffers.clear();
    }

    void recordCommandBuffers()
    {
        // auto vkSwapChain = dynamic_cast<VulkanSwapChain *>(this->_context->getSwapChain());
        // const auto &extents = vkSwapChain->getImageExtents();
        // // build command buffer
        // // each swapchain have 1 command buffer, needs to build it
        // // VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT: sumbit to queue while previous submit is not ended
        // // pInheritanceInfo" nullptr -> primany command buffer
        // for (int32_t i = 0; i < static_cast<int32_t>(commandBuffers.size()); ++i)
        // {
        //     // framebuffer connectes the imageView to the renderpass's attachments
        //     const auto &cmd = commandBuffers[i];
        //     VkFramebuffer framebuffer = this->swapchain_framebuffers[i];

        //     const VkCommandBufferBeginInfo command_buffer_begin_info =
        //         {
        //             .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        //             .pNext = nullptr,
        //             .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        //             .pInheritanceInfo = nullptr};

        //     VK_CHECK(vkBeginCommandBuffer(cmd, &command_buffer_begin_info));
        //     VkClearValue clear_value;
        //     clear_value.color = {{0.01f, 0.01f, 0.033f, 1.0f}};
        //     // if you have multiple render passes, you can go forward to the next pass
        //     VkRenderPassBeginInfo rp_begin{
        //         .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        //         .renderPass = this->render_pass,
        //         .framebuffer = framebuffer,
        //         .renderArea{
        //             .extent{
        //                 .width = extents.width,
        //                 .height = extents.height,
        //             }},
        //         .clearValueCount = 1,
        //         .pClearValues = &clear_value,

        //     };
        //     vkCmdBeginRenderPass(cmd, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);
        //     vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pipeline);
        //     VkViewport vp{
        //         .width = extents.width,
        //         .height = extents.height,
        //         .minDepth = 0.f,
        //         .maxDepth = 1.f,
        //     };
        //     // dynamic state of viewport is enabled when pipeline is created
        //     vkCmdSetViewport(cmd, 0, 1, &vp);

        //     VkRect2D scissor{
        //         .extent{
        //             .width = extents.width,
        //             .height = extents.height,
        //         }};
        //     // dynamic state of scissor is enabled when pipeline is created
        //     vkCmdSetScissor(cmd, 0, 1, &scissor);
        //     vkCmdDraw(cmd, 3, 1, 0, 0);
        //     vkCmdEndRenderPass(cmd);
        //     VK_CHECK(vkEndCommandBuffer(cmd));
        // }
        // // submitting command buffers to a queue
        // // leve the variable field to be updated in the rendering loop
        // const VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}; // or even VERTEX_SHADER_STAGE

        // submit_info = {
        //     .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        //     .pNext = nullptr,
        //     .waitSemaphoreCount = 1,
        //     .pWaitSemaphores = &acquired_image_ready,
        //     .pWaitDstStageMask = waitStages,
        //     .signalSemaphoreCount = 1,
        //     .pSignalSemaphores = &render_complete};
    }

    void cleanup()
    {
        // auto vkDevice = device->getVkHandle();
        // vkDeviceWaitIdle(vkDevice);

        // destroyFramebuffer();

        // destroyCommandBuffers();

        // destroySync();

        // destroyPipeline();

        // destroyRenderPass();

        // // swap chain resource is done through the virtual ctor of wrapper class
        // // device is clean in the wrapper class
    }

    // // rendering process waits for swap chain image
    // VkSemaphore acquired_image_ready;

    // // swap chain present waits for render completes
    // VkSemaphore render_complete;

    // // Synchronization fences
    // std::vector<VkFence> wait_fences;

    // // Active image index in the swapchain
    // uint32_t current_image_index = 0;

    // VulkanLogicalDevice *device;

    // // Command buffer pool
    // VkCommandPool cmd_pool;

    // // Command buffer vector size matches swapchain image array's size
    // vector<VkCommandBuffer> commandBuffers;

    // // Contains command buffers and semaphores to be presented to the queue
    // VkSubmitInfo submit_info;

    // VkQueue queue{VK_NULL_HANDLE};

    // VkRenderPass render_pass{VK_NULL_HANDLE};

    // VkPipelineLayout pipelineLayout{VK_NULL_HANDLE};

    // VkPipeline pipeline{VK_NULL_HANDLE};

    // std::vector<VkFramebuffer> swapchain_framebuffers;
};

int main()
{
    auto framework_module = []
    {
        return di::make_injector(
            di::bind<IWindow>().to<GlfwWindow>(),
            di::bind<IApplication>().to<VulkanApplication>());
    };
    auto injector = di::make_injector(framework_module());
    auto platform = injector.create<std::unique_ptr<UnixPlatform>>();
    platform.get()->main_loop();
    return 0;
}