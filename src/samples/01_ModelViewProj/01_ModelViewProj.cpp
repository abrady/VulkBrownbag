#include "Vulk.h"
#include "VulkMesh.h"
#include "VulkGeo.h"
#include "VulkPipelineBuilder.h"
#include "VulkBufferBuilder.h"
#include "VulkDescriptorSetLayoutBuilder.h"
#include "VulkDescriptorSetBuilder.h"

// Super basic example that is basically the minimum needed to draw a triangle
// Challenges:
// 1. open this up in renderdoc and debug the vertex and fragment shaders
// 2. change the color of the triangle and clear color
// 3. change the size of the triangle so it fits the entire viewport (hint: VulkMesh has an 'xform' method to help with this)
//
// Vulk is a base class that takes care of the boilerplate of setting up a vulkan app
// I wouldn't worry too much about it for now.
class Triangle : public Vulk
{
    std::shared_ptr<VulkPipeline> pipeline;
    std::shared_ptr<VulkDescriptorSetInfo> descriptorSetInfo;

    uint32_t numIndices;
    std::shared_ptr<VulkBuffer> triIndices;
    std::shared_ptr<VulkBuffer> triVerts;

protected:
    virtual void init()
    {
        VulkMesh tri;

        // the descriptor set layout is a list of all the resources that the shader will use
        // in this case it is empty but you'll want to pass in things like the view matrix eventually
        descriptorSetInfo = VulkDescriptorSetBuilder(*this)
                                .build();

        // the pipeline defines how your input data is processed and output to screen
        // it needs to know what the input data looks like (vertex input), what shaders to use,
        // what fixed features we're using (depth, culling) and how to blend the output
        pipeline = VulkPipelineBuilder(*this)
                       .addVertexInput(0, VK_FORMAT_R32G32B32_SFLOAT, sizeof(tri.vertices[0]))
                       .addvertShaderStage("passthru.vert")
                       .addFragmentShaderStage("passthru.frag")
                       .setPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
                       .setCullMode(VK_CULL_MODE_BACK_BIT)
                       .setDepthTestEnabled(true)
                       .setDepthWriteEnabled(true)
                       .setDepthCompareOp(VK_COMPARE_OP_LESS)
                       .setScissor(swapChainExtent)
                       .setViewport(swapChainExtent)
                       .build(renderPass, descriptorSetInfo->descriptorSetLayout);

        makeEquilateralTri(1.f, 1, tri);
        numIndices = (uint32_t)tri.indices.size();

        // buffers are used to store data on the gpu
        // - the usage flags tell vulkan what the buffer will be used for
        // - VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT: the buffer will be used by the gpu

        triIndices = VulkBufferBuilder(*this)
                         .setSize(sizeof(tri.indices[0]) * tri.indices.size())
                         .setMem(tri.indices.data())
                         .setUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
                         .setProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
                         .build();

        triVerts = VulkBufferBuilder(*this)
                       .setSize(sizeof(tri.vertices[0]) * tri.vertices.size())
                       .setMem(tri.vertices.data())
                       .setUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
                       .setProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
                       .build();
    }
    virtual void drawFrame(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        VK_CALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = frameBuffer;
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.1f, 0.0f, 0.1f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChainExtent.width;
        viewport.height = (float)swapChainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChainExtent;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipeline);
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->pipelineLayout, 0, 1,
                                    &descriptorSetInfo->descriptorSets[currentFrame]->descriptorSet, 0, nullptr);

            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, &triVerts->buf, offsets);
            vkCmdBindIndexBuffer(commandBuffer, triIndices->buf, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(commandBuffer, numIndices, 1, 0, 0, 0);
        }
        vkCmdEndRenderPass(commandBuffer);

        VK_CALL(vkEndCommandBuffer(commandBuffer));
    }
    virtual void cleanup(){};
};

int main()
{
    Triangle app;
    app.run();
    return 0;
}