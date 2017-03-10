find_path(GLM_INCLUDE_DIRS "glm/glm.hpp"
	PATHS $ENV{GLM_HOME} 
	${GLM_HOME}
	"/usr/include"
    "/usr/local/include")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLM DEFAULT_MSG
                                  GLM_INCLUDE_DIRS)