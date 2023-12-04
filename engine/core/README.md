# Build Core Engine

## VK instance

### Dependencies

1. required instance ext

required instance extension  may not be supported by driver or gpu

2. required validation layers


Debugger

All the extensions regarding debug

VK_EXT_DEBUG_UTILS_EXTENSION_NAME
VK_EXT_DEBUG_REPORT_EXTENSION_NAME

Extenstions for portablility

VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME
VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME

All the validation layers

// The preferred validation layer is "VK_LAYER_KHRONOS_validation"
	        {"VK_LAYER_KHRONOS_validation"},

	        // Otherwise we fallback to using the LunarG meta layer
	        {"VK_LAYER_LUNARG_standard_validation"},

	        // Otherwise we attempt to enable the individual layers that compose the LunarG meta layer since it doesn't exist
	        {
	            "VK_LAYER_GOOGLE_threading",
	            "VK_LAYER_LUNARG_parameter_validation",
	            "VK_LAYER_LUNARG_object_tracker",
	            "VK_LAYER_LUNARG_core_validation",
	            "VK_LAYER_GOOGLE_unique_objects",
	        },

	        // Otherwise as a last resort we fallback to attempting to enable the LunarG core layer
	        {"VK_LAYER_LUNARG_core_validation"}};



## VK physical device



