# Build Core Engine

## Step1: VK instance

## Layers (like middleware)
https://vulkan.lunarg.com/doc/view/latest/windows/layer_configuration.html

Method selected in this repo: Enabling and ordering the layer using vkCreateInstance()


### Relationship between layers and extensions
When pLayerName parameter is NULL, only extensions provided by the Vulkan implementation or by implicitly enabled layers are returned. When pLayerName is the name of a layer, the instance extensions provided by that layer are returned.


extensions are grouped into layers
Check the 1st parameters of vkEnumerateInstanceExtensionProperties: pLayerName

### Dependencies

1. required instance ext

required instance extension  may not be supported by driver or gpu

2. required validation layers


Debugger

All the extensions regarding debug

New: VK_EXT_DEBUG_UTILS_EXTENSION_NAME

Deprecated: VK_EXT_DEBUG_REPORT_EXTENSION_NAME


### Relation between debug ext and instance (as instance ext)

To capture events that occur while creating or destroying an instance, an application can link a VkDebugReportCallbackCreateInfoEXT structure or a VkDebugUtilsMessengerCreateInfoEXT structure to the pNext element of the VkInstanceCreateInfo structure given to vkCreateInstance. This callback is only valid for the duration of the vkCreateInstance and the vkDestroyInstance call. Use vkCreateDebugReportCallbackEXT or vkCreateDebugUtilsMessengerEXT to create persistent callback objects.

### Device Compatability (MacOs)
If using MacOS with the latest MoltenVK sdk, you may get VK_ERROR_INCOMPATIBLE_DRIVER returned from vkCreateInstance. According to the Getting Start Notes. Beginning with the 1.3.216 Vulkan SDK, the VK_KHR_PORTABILITY_subset extension is mandatory.

To get over this error, first add the VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR bit to VkInstanceCreateInfo struct's flags, then add VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME to instance enabled extension list.


Extenstions for portablility

VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME


### Best Practice Validation (instance ext)
Validation layer to check if your implementation follows best practice
Layer: VK_LAYER_KHRONOS_validation
Extenstion within layer: VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
VkValidationFeaturesEXT: 

### Headless
VK_EXT_headless_surface - instance extension

## Step2: Platform-dependent VKSurface

Surface wrapper ? 
depends:
1. Instance 
2. Window


## Step3: VK physical device manager
depends: 
1. Instance


// 2. Surface

### Get List of physical devices ? 
vkEnumeratePhysicalDevices

    VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU = 1,
    VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU = 2,
    VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU = 3,
    VK_PHYSICAL_DEVICE_TYPE_CPU = 4,
    VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM = 0x7FFFFFFF

### How to select device from the list ? 
glfw create VKSurface from the platform (os-dependent)

select physical device based on VKSurface


### Physical Devices Features and Rendering algorithms


## Step4: VK Logic Device
depends:
1. PhysicalDeviceManager to select
2. OS-dependent Surface
3. gpu device extension

extensisons:
1. VK_KHR_SWAPCHAIN_EXTENSION_NAME (if not headless rendering)
2. VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME
3. VK_EXT_DEBUG_MARKER_EXTENSION_NAME


To Create Logic Device: 
1. specify how many queueFamily to enable for this device. This decides graphics/compute/transfer caps.
queueFamily: graphics / compute priority
queueFamily only support compute, only support memory transfer, or multiple-purpose

2. specify enabled device features

### Default device features
vkGetPhysicalDeviceFeatures


### Fine-grained features 
Pre-requisite for enable feature:

1. Check instance extension: VK_KHR_get_physical_device_properties2
2. Check tenshe device extension
3. Check the feature supported or not
4. if supported enabled it before creating logical device

Before enable features, check if the corresponding device extension is supported
Before enable 

linked list to chain device features, which will be passed to logic device create info

1. is a fine-grained feature supported ? 

first check if the device extension is supported in the device !

included in the pNext chain of the VkPhysicalDeviceFeatures2 structure passed to vkGetPhysicalDeviceFeatures2, it is filled in to indicate whether each corresponding feature is supported

2. enable fine-grained feature ? 
be used in the pNext chain of VkDeviceCreateInfo to selectively enable these features

### Ways to enable device features (https://docs.vulkan.org/spec/latest/chapters/features.html)
1. instance extension: VK_KHR_get_physical_device_properties2 

2. Some Device features to mention:
VkPhysicalDeviceHostQueryResetFeatures



## Step5: RenderContext for a window/canvas

depends: 
1. A logic device

SwapChain: vector of images
CommandBuffer