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

## Step2: Platform-dependent VKSurface

Surface wrapper ? 
depends:
1. Instance 
2. Window


## Step2: VK physical device

depends: 
1. Instance
2. Surface

### How to select device from the list ? 

glfw create VKSurface from the platform (os-dependent)

select physical device based on VKSurface





