#ifndef IDX_ERROR_CPP_H
#define IDX_ERROR_CPP_H

extern "C"{
    #include "idx_error_c.h"
}
#include <string>

namespace idx {
    
    /**
     * Get all error as a string to debugging
     */
    std::string idx_get_error_as_string(idx_type_error error){
        switch (error)
        {
            case idx_type_error::INVALID_VALUES:
            return "idx_type_error::INVALID_VALUES";
            case idx_type_error::INVALID_FILE:
            return "idx_type_error::INVALID_FILE";
            case idx_type_error::NOT_ENOUGHT_MEMORY:
            return "idx_type_error::NOT_ENOUGHT_MEMORY";
            case idx_type_error::OUT_OF_BOUND:
            return "idx_type_error::OUT_OF_BOUND";
            default:
                return "idx_type_error::NO_ERROR";
        }
    }
    
    struct idx_error_message {
        size_t length;
        std::string message[];
    };
}


#endif
