#ifndef _CLT_PYTHON_INTERFACE_H_
#define _CLT_PYTHON_INTERFACE_H_

#include "../CLTBaseClass.h"
#include <string>
#include <vector>
#include <map>

// Python includes would normally be here, but we'll use forward declarations instead
struct _object;
typedef _object PyObject;
struct _ts;
typedef _ts PyThreadState;

/**
 * @brief Python scripting interface
 * 
 * The CLTPythonInterface provides integration with the Python interpreter,
 * allowing game systems to execute Python scripts and expose functionality
 * to the scripting environment.
 */
class CLTPythonInterface : public CLTBaseClass {
public:
    /**
     * @brief Default constructor
     */
    CLTPythonInterface();
    
    /**
     * @brief Virtual destructor
     */
    virtual ~CLTPythonInterface();
    
    /**
     * @brief Initialize the Python interface
     * 
     * @param pInitParams Initialization parameters
     * @return true if initialization succeeded, false otherwise
     */
    virtual bool Init(void* pInitParams = nullptr) override;
    
    /**
     * @brief Clean up resources used by the Python interface
     */
    virtual void Term() override;
    
    /**
     * @brief Get the class name
     * 
     * @return The class name as a string
     */
    virtual const char* GetClassName() const override;
    
    /**
     * @brief Initialize the Python interpreter
     * 
     * @param pProgramName Program name to pass to Python
     * @return true if successful, false otherwise
     */
    bool InitializePython(const char* pProgramName = "Matrix");
    
    /**
     * @brief Finalize the Python interpreter
     */
    void FinalizePython();
    
    /**
     * @brief Add a path to the Python module search path
     * 
     * @param pPath Path to add
     * @return true if successful, false otherwise
     */
    bool AddPath(const char* pPath);
    
    /**
     * @brief Get the Python module search paths
     * 
     * @param pPaths Pointer to vector to receive paths
     * @return Number of paths
     */
    uint32_t GetPaths(std::vector<std::string>* pPaths);
    
    /**
     * @brief Execute a Python script from a file
     * 
     * @param pFilename Path to the script file
     * @param pGlobals Optional globals dictionary
     * @param pLocals Optional locals dictionary
     * @return true if successful, false otherwise
     */
    bool ExecuteFile(const char* pFilename, PyObject* pGlobals = nullptr, PyObject* pLocals = nullptr);
    
    /**
     * @brief Execute a Python string
     * 
     * @param pScript Python code to execute
     * @param pGlobals Optional globals dictionary
     * @param pLocals Optional locals dictionary
     * @return true if successful, false otherwise
     */
    bool ExecuteString(const char* pScript, PyObject* pGlobals = nullptr, PyObject* pLocals = nullptr);
    
    /**
     * @brief Call a Python function
     * 
     * @param pModule Module name
     * @param pFunction Function name
     * @param pArgs Arguments tuple
     * @param pResult Optional pointer to receive the result
     * @return true if successful, false otherwise
     */
    bool CallFunction(const char* pModule, const char* pFunction, PyObject* pArgs, PyObject** pResult = nullptr);
    
    /**
     * @brief Get a Python module
     * 
     * @param pName Module name
     * @return Pointer to the module, or nullptr if not found
     */
    PyObject* GetModule(const char* pName);
    
    /**
     * @brief Import a Python module
     * 
     * @param pName Module name
     * @return Pointer to the imported module, or nullptr on error
     */
    PyObject* ImportModule(const char* pName);
    
    /**
     * @brief Register a C++ function as a Python method
     * 
     * @param pModuleName Module name
     * @param pFunctionName Function name
     * @param pFunction Function pointer
     * @param pDocString Optional documentation string
     * @return true if successful, false otherwise
     */
    bool RegisterFunction(const char* pModuleName, const char* pFunctionName, void* pFunction, const char* pDocString = nullptr);
    
    /**
     * @brief Create a new Python module
     * 
     * @param pName Module name
     * @param pDocString Optional documentation string
     * @return Pointer to the new module, or nullptr on error
     */
    PyObject* CreateModule(const char* pName, const char* pDocString = nullptr);
    
    /**
     * @brief Create a Python integer
     * 
     * @param value Integer value
     * @return Pointer to the new Python object
     */
    PyObject* CreateInt(int value);
    
    /**
     * @brief Create a Python float
     * 
     * @param value Float value
     * @return Pointer to the new Python object
     */
    PyObject* CreateFloat(float value);
    
    /**
     * @brief Create a Python string
     * 
     * @param pString String value
     * @return Pointer to the new Python object
     */
    PyObject* CreateString(const char* pString);
    
    /**
     * @brief Create a Python tuple
     * 
     * @param size Tuple size
     * @return Pointer to the new Python tuple
     */
    PyObject* CreateTuple(uint32_t size);
    
    /**
     * @brief Create a Python list
     * 
     * @param size List size
     * @return Pointer to the new Python list
     */
    PyObject* CreateList(uint32_t size);
    
    /**
     * @brief Create a Python dictionary
     * 
     * @return Pointer to the new Python dictionary
     */
    PyObject* CreateDict();
    
    /**
     * @brief Set a tuple item
     * 
     * @param pTuple Tuple to modify
     * @param index Item index
     * @param pValue Value to set
     * @return true if successful, false otherwise
     */
    bool SetTupleItem(PyObject* pTuple, uint32_t index, PyObject* pValue);
    
    /**
     * @brief Set a list item
     * 
     * @param pList List to modify
     * @param index Item index
     * @param pValue Value to set
     * @return true if successful, false otherwise
     */
    bool SetListItem(PyObject* pList, uint32_t index, PyObject* pValue);
    
    /**
     * @brief Set a dictionary item
     * 
     * @param pDict Dictionary to modify
     * @param pKey Key
     * @param pValue Value
     * @return true if successful, false otherwise
     */
    bool SetDictItem(PyObject* pDict, PyObject* pKey, PyObject* pValue);
    
    /**
     * @brief Get a tuple item
     * 
     * @param pTuple Tuple to access
     * @param index Item index
     * @return Pointer to the tuple item
     */
    PyObject* GetTupleItem(PyObject* pTuple, uint32_t index);
    
    /**
     * @brief Get a list item
     * 
     * @param pList List to access
     * @param index Item index
     * @return Pointer to the list item
     */
    PyObject* GetListItem(PyObject* pList, uint32_t index);
    
    /**
     * @brief Get a dictionary item
     * 
     * @param pDict Dictionary to access
     * @param pKey Key
     * @return Pointer to the dictionary item
     */
    PyObject* GetDictItem(PyObject* pDict, PyObject* pKey);
    
    /**
     * @brief Check if a Python object is None
     * 
     * @param pObject Object to check
     * @return true if the object is None, false otherwise
     */
    bool IsNone(PyObject* pObject);
    
    /**
     * @brief Convert a Python object to a string
     * 
     * @param pObject Object to convert
     * @param pResult Pointer to receive the string
     * @return true if successful, false otherwise
     */
    bool ToString(PyObject* pObject, std::string* pResult);
    
    /**
     * @brief Convert a Python object to an integer
     * 
     * @param pObject Object to convert
     * @param pResult Pointer to receive the integer
     * @return true if successful, false otherwise
     */
    bool ToInt(PyObject* pObject, int* pResult);
    
    /**
     * @brief Convert a Python object to a float
     * 
     * @param pObject Object to convert
     * @param pResult Pointer to receive the float
     * @return true if successful, false otherwise
     */
    bool ToFloat(PyObject* pObject, float* pResult);
    
    /**
     * @brief Get the Python interpreter's global dictionary
     * 
     * @return Pointer to the global dictionary
     */
    PyObject* GetGlobals();
    
    /**
     * @brief Get the Python interpreter thread state
     * 
     * @return Pointer to the thread state
     */
    PyThreadState* GetThreadState();
    
    /**
     * @brief Get the last Python error message
     * 
     * @return Error message string
     */
    std::string GetLastError();
    
    /**
     * @brief Clear the last Python error
     */
    void ClearError();
    
    /**
     * @brief Print the Python stack trace to the console
     */
    void PrintStackTrace();
    
    /**
     * @brief Check if a Python module is available
     * 
     * @param pName Module name
     * @return true if the module is available, false otherwise
     */
    bool IsModuleAvailable(const char* pName);
    
    /**
     * @brief Get the Python version string
     * 
     * @return Python version string
     */
    std::string GetPythonVersion();
    
    /**
     * @brief Check if the Python interpreter is initialized
     * 
     * @return true if initialized, false otherwise
     */
    bool IsInitialized() const;
    
    /**
     * @brief Set a global variable in the Python environment
     * 
     * @param pName Variable name
     * @param pValue Variable value
     * @return true if successful, false otherwise
     */
    bool SetGlobal(const char* pName, PyObject* pValue);
    
    /**
     * @brief Get a global variable from the Python environment
     * 
     * @param pName Variable name
     * @return Pointer to the variable, or nullptr if not found
     */
    PyObject* GetGlobal(const char* pName);

private:
    // Internal helper methods
    void InitializeMonolith();
    
    // Python state
    PyThreadState* m_pMainThreadState;
    bool m_initialized;
    std::map<std::string, PyObject*> m_modules;
};

#endif // _CLT_PYTHON_INTERFACE_H_