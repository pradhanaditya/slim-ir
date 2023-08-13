#include "Operand.h"

OperandType SLIMOperand::processOperand(llvm::Value *value)
{
    // Set the operand type
    if (value != nullptr)
    {
        // First check if the operand has a name, which won't be the case when the operand is a 
        // GetElementPtr (GEP) operand for example, because in this case we have to extract the relevant
        // operand and indices from the GEP operand.    
        if (llvm::isa<llvm::GEPOperator>(value))
        {
            return OperandType::GEP_OPERATOR;
        }
        else if (llvm::isa<llvm::AddrSpaceCastOperator>(value))
        {
            return OperandType::ADDR_SPACE_CAST_OPERATOR;
        }
        else if (llvm::isa<llvm::BitCastOperator>(value))
        {
            return OperandType::BITCAST_OPERATOR;
        }
        else if (llvm::isa<llvm::PtrToIntOperator>(value))
        {
            return OperandType::PTR_TO_INT_OPERATOR;
        }
        else if (llvm::isa<llvm::ZExtOperator>(value))
        {
            return OperandType::ZEXT_OPERATOR;
        }
        else if (llvm::isa<llvm::FPMathOperator>(value))
        {
            return OperandType::FP_MATH_OPERATOR;
        }
        else if (llvm::isa<llvm::Constant>(value))
        {
            if (llvm::isa<llvm::BlockAddress>(value))
            {
                return OperandType::BLOCK_ADDRESS;
            }
            else if (llvm::isa<llvm::ConstantAggregate>(value))
            {
                return OperandType::CONSTANT_AGGREGATE;
            }
            else if (llvm::isa<llvm::ConstantDataSequential>(value))
            {
                return OperandType::CONSTANT_DATA_SEQUENTIAL;
            }
            else if (llvm::isa<llvm::ConstantPointerNull>(value))
            {
                return OperandType::CONSTANT_POINTER_NULL;
            }
            else if (llvm::isa<llvm::ConstantTokenNone>(value))
            {
                return OperandType::CONSTANT_TOKEN_NONE;
            }
            else if (llvm::isa<llvm::UndefValue>(value))
            {
                return OperandType::UNDEF_VALUE;
            }
            else if (llvm::isa<llvm::ConstantInt>(value))
            {
                return OperandType::CONSTANT_INT;
            }
            else if (llvm::isa<llvm::ConstantFP>(value))
            {
                return OperandType::CONSTANT_FP;
            }
            else if (llvm::isa<llvm::DSOLocalEquivalent>(value))
            {
                return OperandType::DSO_LOCAL_EQUIVALENT;
            }
            else if (llvm::isa<llvm::GlobalValue>(value))
            {
                return OperandType::GLOBAL_VALUE;
            }
            else if (llvm::isa<llvm::NoCFIValue>(value))
            {
                return OperandType::NO_CFI_VALUE;
            }
            else
            {
                return OperandType::NOT_SUPPORTED_OPERAND;
            }
        }
        else if (value->hasName())
        {
            return OperandType::VARIABLE;
        }
        else
        {
            return OperandType::NOT_SUPPORTED_OPERAND;
        }
    }
    else
    {
        return OperandType::NULL_OPERAND;
    }
}

// Operand is not a address-taken local variable
SLIMOperand::SLIMOperand(llvm::Value *value)
{
    this->value = value;
    this->is_global_or_address_taken = false;
    this->is_pointer_variable = false;
    this->gep_main_operand = nullptr;
    this->has_indices = false;
    this->has_name = false;
    
    this->is_ssa_version = false;
    this->ssa_version_number = 0;

    this->is_array_type = false;

    if (value != nullptr)
    {
        /*
            Since every global and address taken variable in the LLVM IR (after mem2reg pass) is 
            a pointer, we distinguish pointers in the source program from the pointers in LLVM by
            checking whether a LLVM pointer is double pointer or not. 
            
            But we have to be careful regarding this because there might be some instances in which we 
            can interpret the operand incorrectly. For example, a GetElementPtr (GEP) operand is always 
            a pointer but if we try to check this by taking the operand (of GEPOperand) then we may get 
            invalid results because that operand may or may not be a pointer.
        */

        if (llvm::isa<llvm::GlobalValue>(value))
        {
            if (llvm::isa<llvm::PointerType>(llvm::cast<llvm::GlobalValue>(value)->getType()))
            {
                this->is_pointer_variable = true;
            }
        }
        else if (value->getType()->getNumContainedTypes() > 0 && value->getType()->getContainedType(0)->isPointerTy())
        {
            this->is_pointer_variable = true;
        }

        if (value->hasName())
        {
            this->has_name = true;
        }

        if (llvm::isa<llvm::Argument>(this->value))
        {
            this->is_formal_argument = true;
        }
    }

    // Get the operand type
    this->operand_type = SLIMOperand::processOperand(this->value);

    // Check if the operand is a GEPOperator and the operand does not have a name
    if (this->operand_type == OperandType::GEP_OPERATOR)
    {
        // Cast the operand to llvm::GEPOperator
        llvm::GEPOperator *gep_operator = llvm::cast<llvm::GEPOperator>(this->value);

        // Get the variable operand (which is the first operand)
        this->gep_main_operand = gep_operator->getOperand(0);

        if (gep_operator->getNumOperands() > 0)
        {
            this->has_indices = true;
        }

        for (int i = 1; i < gep_operator->getNumOperands(); i++)
        {
            llvm::Value *index_val = gep_operator->getOperand(i);

            // Check if the index is constant
            if (llvm::isa<llvm::Constant>(index_val))
            {
                if (llvm::isa<llvm::ConstantInt>(index_val))
                {
                    SLIMOperand * index_operand = new SLIMOperand(index_val);

                    this->indices.push_back(index_operand);
                }
                // If the indices are constant, they must be integers
                else
                {
                    llvm_unreachable("[SLIMOperand error while construction of GEPOperator] The index is a constant but not an integer constant!");
                }
            }
            else if (index_val->hasName())
            {
                SLIMOperand *index_operand = new SLIMOperand(index_val);

                this->indices.push_back(index_operand);
            }
            else
            {
                // The index is stored in a variable or SSA register
                llvm_unreachable("[SLIMOperand error while construction of GEPOperator] The index is not a constant!");
            }
        }        
    }
}

// Operand may or may not be a address-taken local or global variable
SLIMOperand::SLIMOperand(llvm::Value *value, bool is_global_or_address_taken, llvm::Function *direct_callee_function)
{
    this->value = value;
    this->is_global_or_address_taken = is_global_or_address_taken;
    this->direct_callee_function = direct_callee_function;
    this->is_pointer_variable = false;
    this->gep_main_operand = nullptr;
    this->has_indices = false;
    this->has_name = false;

    this->is_ssa_version = false;
    this->ssa_version_number = 0;

    this->is_array_type = false;

    if (value != nullptr)
    {
        if (llvm::isa<llvm::GlobalValue>(value))
        {
            if (llvm::isa<llvm::PointerType>(llvm::cast<llvm::GlobalValue>(value)->getType()))
            {
                this->is_pointer_variable = true;
            }
        }
        // Same argument (described in the above constructor)
        else if (value->getType()->getNumContainedTypes() > 0 && value->getType()->getContainedType(0)->isPointerTy())
        {
            this->is_pointer_variable = true;
        }

        if (value->hasName())
        {
            this->has_name = true;
        }

        if (llvm::isa<llvm::Argument>(this->value))
        {
            this->is_formal_argument = true;
        }
    }

    // Get the operand type
    this->operand_type = SLIMOperand::processOperand(this->value);

    // Check if the operand is a GEPOperator and the operand does not have a name
    if (this->operand_type == OperandType::GEP_OPERATOR)
    {
        // Cast the operand to llvm::GEPOperator
        llvm::GEPOperator *gep_operator = llvm::cast<llvm::GEPOperator>(this->value);

        // Get the variable operand (which is the first operand)
        this->gep_main_operand = gep_operator->getOperand(0);

        if (gep_operator->getNumOperands() > 0)
        {
            this->has_indices = true;
        }

        for (int i = 1; i < gep_operator->getNumOperands(); i++)
        {
            llvm::Value *index_val = gep_operator->getOperand(i);

            // Check if the index is constant
            if (llvm::isa<llvm::Constant>(index_val))
            {
                if (llvm::isa<llvm::ConstantInt>(index_val))
                {
                    SLIMOperand * index_operand = new SLIMOperand(index_val);

                    this->indices.push_back(index_operand);
                }
                // If the indices are constant, they must be integers
                else
                {
                    llvm_unreachable("[SLIMOperand error while construction of GEPOperator] The index is a constant but not an integer constant!");
                }
            }
            else if (index_val->hasName())
            {
                SLIMOperand *index_operand = new SLIMOperand(index_val);

                this->indices.push_back(index_operand);
            }
            else
            {
                // The index is stored in a variable or SSA register
                llvm_unreachable("[SLIMOperand error while construction of GEPOperator] The index is not a constant!");
            }
        }        
    }
}

// Returns the operand type
OperandType SLIMOperand::getOperandType()
{
    return this->operand_type;
}

// Returns true if the operand is a global variable or a address-taken local variable
bool SLIMOperand::isGlobalOrAddressTaken()
{
    return this->is_global_or_address_taken || this->isAlloca();
}

// Returns true if the operand is a formal argument of a function
bool SLIMOperand::isFormalArgument()
{
    return this->is_formal_argument;
}

// Returns true if the operand is a global variable or an address-taken local variable (considers only the struct if the operand is a GEP operator)
bool SLIMOperand::isVariableGlobal()
{
    if (this->operand_type == OperandType::GEP_OPERATOR)
    {
        // Cast the operand to llvm::GEPOperator
        llvm::GEPOperator *gep_operator = llvm::cast<llvm::GEPOperator>(this->value);

        // Get the variable operand (which is the first operand)
        this->gep_main_operand = gep_operator->getOperand(0);

        return llvm::isa<llvm::GlobalValue>(this->gep_main_operand);
    }

    return this->isGlobalOrAddressTaken() || llvm::isa<llvm::GlobalValue>(this->value);
}

// Returns true if the operand is a result of an alloca instruction
bool SLIMOperand::isAlloca()
{
    return OperandRepository::alloca_operand.find(this->value) != OperandRepository::alloca_operand.end();
}

// Returns true if the operand is a pointer variable (with reference to the LLVM IR)
bool SLIMOperand::isPointerInLLVM()
{
    if (llvm::isa<llvm::GlobalValue>(this->value))
    {
        if (llvm::isa<llvm::PointerType>(llvm::cast<llvm::GlobalValue>(this->value)->getValueType()))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return this->value->getType()->isPointerTy();
}

// Returns true if the operand is a pointer variable
bool SLIMOperand::isPointerVariable()
{
    return this->is_pointer_variable;
}

// Returns true if the operand is of array type
bool SLIMOperand::isArrayElement()
{
    if (this->is_array_type)
        return true;

    if (llvm::isa<llvm::GEPOperator>(this->value))
    {
        llvm::GEPOperator *gep_operand = llvm::cast<llvm::GEPOperator>(this->value);

        // Get the type of the GEP main operand
        llvm::Type * type = this->gep_main_operand->getType()->getContainedType(0);

        // If the type is an array then return true
        if (type->isArrayTy())
            return true;

        // Otherwise the type is a structure, so we check if any of its fields is of array type
        for (unsigned i = 2; i < gep_operand->getNumOperands(); i++)
        {
            // Get the ith operand
            llvm::Value * operand_i = gep_operand->getOperand(i);

            // Cast the value object to constant int (since it is an index)
            llvm::ConstantInt *constant_int_i = llvm::cast<llvm::ConstantInt>(operand_i);

            // If the type of GEPOperator is a structure
            if (llvm::isa<llvm::StructType>(type))
            {
                // Assignment to type is required to check for nested structures
                type = type->getStructElementType(constant_int_i->getSExtValue());

                if (type->isArrayTy())
                    return true;
            }
        }
    }

    return false;
}

// Sets the is_array_type to true
void SLIMOperand::setArrayType()
{
    this->is_array_type = true;
}

// Sets the gep_main_operand
void SLIMOperand::setGEPMainOperand(SLIMOperand *operand)
{
    this->gep_main_operand = operand->getValue();
}

// Returns true if the operand is a GetElementPtr operand inside an instruction
bool SLIMOperand::isGEPInInstr()
{
    return llvm::isa<llvm::GEPOperator>(this->value) && !this->value->hasName();
}
    
// Sets the is_pointer_variable to true
void SLIMOperand::setIsPointerVariable()
{
    this->is_pointer_variable = true;
}

// Sets the is_pointer_variable to false
void SLIMOperand::unsetIsPointerVariable()
{
    this->is_pointer_variable = false;
}

// Returns the pointer to the corresponding llvm::Value object
llvm::Value* SLIMOperand::getValue()
{
    return this->value;
}

// Returns the type of the operand
llvm::Type * SLIMOperand::getType()
{
    if (this->value && (llvm::isa<llvm::GlobalValue>(this->value) || this->isAlloca()))
    {
        return this->value->getType()->getContainedType(0);
    }

    if (this->value)
    {
        return this->value->getType();
    }

    return nullptr;
}

// Returns the number of indices
unsigned SLIMOperand::getNumIndices()
{
    return this->indices.size();
}

// Returns if the operand has a name
bool SLIMOperand::hasName()
{
    return this->has_name;
}

 // Returns the operand index at the specified position (0-based position)
SLIMOperand * SLIMOperand::getIndexOperand(unsigned position)
{
    // Check if the indices exist and the position (0-based) is in bounds or not
    assert(this->has_indices && position >= 0 && position < this->getNumIndices());

    // Return the index operand
    return this->indices[position];
}

// Returns the vector of indices
std::vector<SLIMOperand *> SLIMOperand::getIndexVector()
{
    return this->indices;
}

void SLIMOperand::addIndexOperand(SLIMOperand * indOperand)
{
    //Add new index operand to indices
    this->has_indices = true;
    this->indices.push_back(indOperand);
}

// Internal function to be used only in case of print related tasks
std::string SLIMOperand::_getOperandName()
{
    llvm::Value *operand = this->getValue();
    
    // This will hold the string value of the operand
    std::string operand_name;

    // The string in this stream will be flushed into the operand_name
    llvm::raw_string_ostream stream(operand_name);
    
    // First check if the operand has a name, which won't be the case when the operand is a 
    // GetElementPtr (GEP) operand for example, because in this case we have to extract the relevant
    // operand and indices from the GEP operand 
    if (operand->hasName())
    {
        stream << operand->getName();
    }
    else if (llvm::isa<llvm::GEPOperator>(operand))
    {
        // Cast the operand to llvm::GEPOperator
        llvm::GEPOperator *gep_operator = llvm::cast<llvm::GEPOperator>(operand);

        // Get the variable operand (which is the first operand)
        llvm::Value *gep_operand = gep_operator->getOperand(0);

        // Print the variable name
        stream << gep_operand->getName();

        // Print the indices
        for (int i = 1; i < gep_operator->getNumOperands(); i++)
        {
            llvm::Value *index_val = gep_operator->getOperand(i);

            // Check if the index is constant
            if (llvm::isa<llvm::Constant>(index_val))
            {
                if (llvm::isa<llvm::ConstantInt>(index_val))
                {
                    llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(index_val);

                    // Print the constant integer
                    stream << "[" << constant_int->getSExtValue() << "]";
                }
                // If the indices are constant, they must be integers
                else
                {
                    llvm_unreachable("[GetElementPtrOperator Error] The index is a constant but not an integer constant!");
                }
            }
            else
            {
                // The index is stored in a variable or SSA register
                stream << "[" << index_val->getName() << "]";
            }
        }
    }
    else if (llvm::isa<llvm::AddrSpaceCastOperator>(operand))
    {
        llvm::cast<llvm::AddrSpaceCastOperator>(operand)->print(stream);
    }
    // else if (llvm::isa<llvm::AddrSpaceCastInst>(operand))
    // {
    //     llvm::cast<llvm::AddrSpaceCastInst>(operand)->print(stream);
    // }
    else if (llvm::isa<llvm::BitCastOperator>(operand))
    {
        llvm::BitCastOperator *bitcast_operator = llvm::cast<llvm::BitCastOperator>(operand);

        bitcast_operator->print(stream);
    }
    else if (llvm::isa<llvm::PtrToIntOperator>(operand))
    {
        llvm::cast<llvm::PtrToIntOperator>(operand)->print(stream);
    }
    else if (llvm::isa<llvm::ZExtOperator>(operand))
    {
        llvm::cast<llvm::ZExtOperator>(operand)->print(stream);
    }
    else if (llvm::isa<llvm::FPMathOperator>(operand))
    {
        llvm::cast<llvm::FPMathOperator>(operand)->print(stream);
    }
    else if (llvm::isa<llvm::Constant>(operand))
    {
        if (llvm::isa<llvm::BlockAddress>(operand))
        {
            llvm::BlockAddress *block_address = llvm::cast<llvm::BlockAddress>(operand);

            stream << block_address->getBasicBlock()->getName();
        }
        else if (llvm::isa<llvm::ConstantAggregate>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support constant aggregate!");
        }
        else if (llvm::isa<llvm::ConstantDataSequential>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support constant data sequential!");
        }
        else if (llvm::isa<llvm::ConstantPointerNull>(operand))
        {
            llvm::ConstantPointerNull *const_pointer_null = llvm::cast<llvm::ConstantPointerNull>(operand);

            if (const_pointer_null)
                stream << const_pointer_null->getName() << " ";
            
            stream << "nullptr";
        }
        else if (llvm::isa<llvm::ConstantTokenNone>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support constant token none!");
        }
        else if (llvm::isa<llvm::UndefValue>(operand))
        {
            stream << "undef";
        }
        else if (llvm::isa<llvm::ConstantInt>(operand))
        {
            llvm::ConstantInt *constant_int = llvm::cast<llvm::ConstantInt>(operand);

            stream << constant_int->getSExtValue();
        }
        else if (llvm::isa<llvm::ConstantFP>(operand))
        {
            llvm::ConstantFP *constant_float = llvm::cast<llvm::ConstantFP>(operand);

            stream << constant_float->getValueAPF().convertToFloat();
        }
        else if (llvm::isa<llvm::DSOLocalEquivalent>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support DSO local equivalent!");
        }
        else if (llvm::isa<llvm::GlobalValue>(operand))
        {
            stream << llvm::cast<llvm::GlobalValue>(operand)->getName();
        }
        else if (llvm::isa<llvm::NoCFIValue>(operand))
        {
            llvm_unreachable("[SLIMOperand Error] The print function does not support NoCFIValue!");
        }
        else if (llvm::isa<llvm::ConstantExpr>(operand))
        {
            stream << llvm::cast<llvm::ConstantExpr>(operand)->getName();
        }
        else
        {
            llvm_unreachable("[SLIMOperand Error] Unexpected constant!");
        }
    }
    else
    {
        // operand->print(llvm::outs());
        if (operand->hasName())
        {
            stream << operand->getName();
        }
        else
        {
            stream << "noname";
            // llvm_unreachable("[SLIMOperand Error] Unexpected operand!");
        }
    }

    std::string result = std::string(stream.str());
    
    if (this->is_ssa_version)
    {
        result += ("_" + std::to_string(this->ssa_version_number));
    }
    //stream.flush();
    
    return result;
}

// Print the SLIM operand
void SLIMOperand::printOperand(llvm::raw_ostream &stream)
{
    stream << llvm::StringRef(this->_getOperandName());

    return ;
}

// Sets the SSA version
void SLIMOperand::setSSAVersion(unsigned ssa_version)
{
    //llvm::outs() << "Setting SSA Version...\n";
    this->is_ssa_version = true;
    this->ssa_version_number = ssa_version;
}

void SLIMOperand::resetSSAVersion()
{
    this->is_ssa_version = false;
}

// Returns the "return operand" of the callee function if this operand is the result of a "direct" call instruction
SLIMOperand * SLIMOperand::getCalleeReturnOperand()
{
    if (this->direct_callee_function == nullptr)
    {
        return nullptr;
    }
    else
    {
        SLIMOperand *return_operand = OperandRepository::getFunctionReturnOperand(this->direct_callee_function);
        
        return return_operand;
    }
}

// Clears the index vector
void SLIMOperand::resetIndexVector()
{
    this->indices.clear();
}

// Sets 'is_global_or_address_taken' to be true for this operand
void SLIMOperand::setVariableGlobal()
{
    this->is_global_or_address_taken = true;
}

// Sets 'is_formal_argument' to be true for this operand
void SLIMOperand::setFormalArgument()
{
    this->is_formal_argument = true;
}

// --------------- APIs for the Legacy SLIM ---------------
    
// Returns the name of the operand
llvm::StringRef SLIMOperand::getName()
{
    std::string ssa_info = "";

    if (this->is_ssa_version)
    {
        llvm::outs() << "Setting SSA Info...\n";
        ssa_info = "_" + std::to_string(this->ssa_version_number);
    }

    std::string *operand_name = new std::string(this->_getOperandName() + ssa_info);
    //llvm::outs() << "Name: " << *operand_name << "\n";
    return llvm::StringRef(*operand_name);
}

// Returns only name for structures (and not indices) in string format and returns 
// the same value as getName for other type of operands
llvm::StringRef SLIMOperand::getOnlyName()
{
    if (this->isGEPInInstr())
    {
        // Cast the operand to llvm::GEPOperator
        llvm::GEPOperator *gep_operator = llvm::cast<llvm::GEPOperator>(this->value);

        // Get the variable operand (which is the first operand)
        llvm::Value *gep_operand = gep_operator->getOperand(0);

        // Return the structure variable name
        return gep_operand->getName();
    }
    else
    {
        return this->getName();
    }
}

// --------------------------------------------------------

// Methods of the OperandRepository namespace

namespace OperandRepository
{
    // Check whether a SLIMOperand object corresponds to a global or a address-taken local variable or not
    std::map<llvm::Value *, SLIMOperand *> value_to_slim_operand;

    // Contains the value objects that are a result of alloca instruction
    std::set<llvm::Value *> alloca_operand;

    // Contains the return operand of every function
    std::map<llvm::Function *, SLIMOperand *> function_return_operand;
};

SLIMOperand * OperandRepository::getSLIMOperand(llvm::Value *value)
{
    if (OperandRepository::value_to_slim_operand.find(value) != OperandRepository::value_to_slim_operand.end())
    {
        return OperandRepository::value_to_slim_operand[value];
    }

    return nullptr;
}

void OperandRepository::setSLIMOperand(llvm::Value *value, SLIMOperand *slim_operand)
{
    OperandRepository::value_to_slim_operand[value] = slim_operand;
}

// Returns the return operand of a function
SLIMOperand * OperandRepository::getFunctionReturnOperand(llvm::Function *function)
{
    return OperandRepository::function_return_operand[function];
}

// Sets the return operand of a function
void OperandRepository::setFunctionReturnOperand(llvm::Function *function, SLIMOperand *return_operand)
{
    OperandRepository::function_return_operand[function] = return_operand;
}