#include "Vulk.h"
#include "VulkMesh.h"
#include "VulkGeo.h"
#include "VulkPipelineBuilder.h"
#include "VulkBufferBuilder.h"
#include "VulkDescriptorSetLayoutBuilder.h"
#include "VulkDescriptorSetBuilder.h"

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
        descriptorSetInfo = VulkDescriptorSetBuilder(*this)
                                .build();
        pipeline = VulkPipelineBuilder(*this)
                       .addVertexInput(0, VK_FORMAT_R32G32B32_SFLOAT, sizeof(tri.vertices[0]))
                       .addvertShaderStage("passthru.vert")
                       .addFragmentShaderStage("passthru.frag")
                       .build(renderPass, descriptorSetInfo->descriptorSetLayout);

        makeEquilateralTri(1.f, 1, tri);
        numIndices = (uint32_t)tri.indices.size();

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