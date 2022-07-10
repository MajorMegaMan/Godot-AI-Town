#ifndef GOAPCORE_H
#define GOAPCORE_H

#include <vector>
#include <queue>
#include <stack>

#define MAX_CAPACITY 200 // RedirectMap max cpacity
#define MAX_DEPTH 50 // max depth for planner

namespace GOAP
{
    class MultiTypeArray
    {
        char* m_allocatedArray = nullptr;
        size_t m_totalSize = 0;

        std::vector<size_t> m_elementSizeArray;

    public:

        MultiTypeArray()
        {

        }

        ~MultiTypeArray()
        {
            if (m_allocatedArray != nullptr)
            {
                delete[] m_allocatedArray;
            }
        }

        MultiTypeArray(const MultiTypeArray& other) = delete;
        MultiTypeArray& operator=(const MultiTypeArray& other) = delete;

    private:
        void AllocateValueArray(size_t totalSize)
        {
            if (m_allocatedArray != nullptr)
            {
                delete[] m_allocatedArray;
            }

            m_totalSize = totalSize;
            m_allocatedArray = new char[totalSize];
        }

        void FinaliseValueSizes()
        {
            if (m_totalSize > 0)
            {
                AllocateValueArray(m_totalSize);
                memset(m_allocatedArray, 0, m_totalSize);
            }
        }

        void AddValueSize(size_t size)
        {
            m_elementSizeArray.push_back(size);
            m_totalSize += size;
        }

    public:

        void AppendValueSize(size_t valueSize)
        {
            if (m_allocatedArray != nullptr)
            {
                char* oldData = m_allocatedArray;
                size_t oldSize = m_totalSize;

                m_totalSize = m_totalSize + valueSize;
                m_allocatedArray = new char[m_totalSize];

                size_t offset = 0;
                for (int i = 0; i < m_elementSizeArray.size(); i++)
                {
                    size_t valueSize = m_elementSizeArray[i];
                    memcpy(m_allocatedArray + offset, oldData + offset, valueSize);
                    offset += valueSize;
                }

                m_elementSizeArray.push_back(valueSize);
                memset(m_allocatedArray + offset, 0, valueSize);
                delete[] oldData;
            }
            else
            {
                AddValueSize(valueSize);
                FinaliseValueSizes();
            }
        }

        void AppendRangeValueSize(size_t* valueSizesArray, size_t arraySize)
        {
            if (m_allocatedArray != nullptr)
            {
                char* oldData = m_allocatedArray;
                size_t oldSize = m_totalSize;

                size_t additionalTotalSize = 0;
                for (int i = 0; i < arraySize; i++)
                {
                    additionalTotalSize += valueSizesArray[i];
                }
                m_totalSize = m_totalSize + additionalTotalSize;
                m_allocatedArray = new char[m_totalSize];

                // copy old elements
                size_t offset = 0;
                for (int i = 0; i < m_elementSizeArray.size(); i++)
                {
                    size_t valueSize = m_elementSizeArray[i];
                    memcpy(m_allocatedArray + offset, oldData + offset, valueSize);
                    offset += valueSize;
                }

                for (int i = 0; i < arraySize; i++)
                {
                    m_elementSizeArray.push_back(valueSizesArray[i]);
                }

                memset(m_allocatedArray + offset, 0, additionalTotalSize);
                delete[] oldData;
            }
            else
            {
                for (int i = 0; i < arraySize; i++)
                {
                    AddValueSize(valueSizesArray[i]);
                }
                FinaliseValueSizes();
            }
        }

        void AppendRangeValueSize(std::vector<size_t> valueSizesArray)
        {
            AppendRangeValueSize(valueSizesArray.begin()._Ptr, valueSizesArray.size());
        }

        void Clear()
        {
            if (m_allocatedArray != nullptr)
            {
                delete[] m_allocatedArray;
                m_allocatedArray = nullptr;
            }
            m_totalSize = 0;
        }

        void SetValueSize(size_t index, size_t size)
        {
            m_elementSizeArray[index] = size;

            m_totalSize = 0;
            for (size_t i = 0; i < m_elementSizeArray.size(); i++)
            {
                m_totalSize += m_elementSizeArray[i];
            }
        }

        size_t GetValueSize(size_t index) const
        {
            return m_elementSizeArray[index];
        }

        size_t CalcValueOffset(size_t index) const
        {
            size_t offset = 0;
            for (int i = 0; i < index; i++)
            {
                offset += m_elementSizeArray[i];
            }
            return offset;
        }

        char* GetValuePtr(size_t index)
        {
            size_t offset = CalcValueOffset(index);
            char* value = m_allocatedArray + offset;
            return value;
        }

        char* GetConstValuePtr(size_t index) const
        {
            size_t offset = CalcValueOffset(index);
            char* value = m_allocatedArray + offset;
            return value;
        }

        template <class T>
        void SetValue(size_t index, const T& value)
        {
            char* targetValue = GetValuePtr(index);
            memcpy(targetValue, &value, sizeof(value));
        }

        template <class T>
        T& GetValue(size_t index)
        {
            char* targetValue = GetValuePtr(index);
            return *(T*)targetValue;
        }
    };

    // This is a simple hash table to redirect one index to another index
    class RedirectMap
    {
        struct IndexElement
        {
            size_t key;
            size_t value;

            IndexElement(size_t key, size_t value)
            {
                this->key = key;
                this->value = value;
            }
        };

        IndexElement** m_elementArray;
        size_t m_maxCapacity;

        size_t m_currentSize = 0;

    public:
        RedirectMap()
        {
            m_maxCapacity = MAX_CAPACITY;
            m_elementArray = new IndexElement * [MAX_CAPACITY];
            for (int i = 0; i < m_maxCapacity; i++)
            {
                m_elementArray[i] = nullptr;
            }
        }

        RedirectMap(size_t capacity)
        {
            if (capacity == 0)
            {
                capacity = 1;
            }

            m_maxCapacity = capacity;
            m_elementArray = new IndexElement * [capacity];
            for (int i = 0; i < m_maxCapacity; i++)
            {
                m_elementArray[i] = nullptr;
            }
        }

        RedirectMap(const RedirectMap& other) = delete;
        RedirectMap operator=(const RedirectMap& other) = delete;

        ~RedirectMap()
        {
            for (int i = 0; i < m_maxCapacity; i++)
            {
                if (m_elementArray[i] != nullptr)
                {
                    delete m_elementArray[i];
                    m_elementArray[i] = nullptr;
                }
            }

            delete[] m_elementArray;
            m_elementArray = nullptr;
        }

    private:
        size_t hashFunc(size_t key) const
        {
            return key % m_maxCapacity;
        }

        bool FindElementHash(size_t key, size_t& out_hash) const
        {
            size_t hash = hashFunc(key);
            size_t rehashCount = 0;

            // find non null element with matching keys
            while (rehashCount < m_maxCapacity)
            {
                if (m_elementArray[hash] != nullptr && m_elementArray[hash]->key == key)
                {
                    break;
                }
                hash = hashFunc(hash + 1);
                rehashCount++;
            }

            if (rehashCount >= m_maxCapacity)
            {
                // key not found in element array
                return false;
            }

            // if we get here element at hash should be non null
            out_hash = hash;
            return true;
        }

    public:
        // Insert element at key. Set value if element exits. does nothing if internal array is full.
        void insert(size_t key, size_t value)
        {
            size_t hash = hashFunc(key);
            size_t rehashCount = 0;

            // find next empty or key equals element key
            while (rehashCount < m_maxCapacity && m_elementArray[hash] != nullptr && m_elementArray[hash]->key != key)
            {
                hash = hashFunc(hash + 1);
                rehashCount++;
            }

            if (rehashCount >= m_maxCapacity)
            {
                // element array is full
                return;
            }

            if (m_elementArray[hash] == nullptr)
            {
                // element is empty
                m_elementArray[hash] = new IndexElement(key, value);
                m_currentSize++;
            }
            else
            {
                // keys are a match
                m_elementArray[hash]->value = value;
            }
        }

        // remove element at key. does nothing if key not found
        void remove(size_t key)
        {
            size_t hash = hashFunc(key);
            size_t rehashCount = 0;

            // find non null element with matching keys
            while (rehashCount < m_maxCapacity)
            {
                if (m_elementArray[hash] != nullptr && m_elementArray[hash]->key == key)
                {
                    break;
                }
                hash = hashFunc(hash + 1);
                rehashCount++;
            }

            if (rehashCount >= m_maxCapacity)
            {
                // key not found in element array
                return;
            }

            // if we get here element at hash should be non null
            delete m_elementArray[hash];
            m_elementArray[hash] = nullptr;
            m_currentSize--;
        }

        // returns value at key. Inserts new if key not found.
        size_t& operator[](size_t key)
        {
            size_t hash = hashFunc(key);
            size_t rehashCount = 0;

            // find next empty or key equals element key
            while (rehashCount < m_maxCapacity && m_elementArray[hash] != nullptr && m_elementArray[hash]->key != key)
            {
                hash = hashFunc(hash + 1);
                rehashCount++;
            }

            if (rehashCount >= m_maxCapacity)
            {
                // element array is full
                size_t invalid = 0;
                return invalid;
            }

            if (m_elementArray[hash] == nullptr)
            {
                // element is empty
                m_elementArray[hash] = new IndexElement(key, 0);
                m_currentSize++;
                return m_elementArray[hash]->value;
            }
            else
            {
                // keys are a match
                return m_elementArray[hash]->value;
            }
        }

        size_t size() const
        {
            return m_currentSize;
        }

        bool contains(size_t key) const
        {
            size_t unused;
            return FindElementHash(key, unused);
        }

        void clear()
        {
            for (int i = 0; i < m_maxCapacity; i++)
            {
                if (m_elementArray[i] != nullptr)
                {
                    delete m_elementArray[i];
                    m_elementArray[i] = nullptr;
                }
            }
        }

        IndexElement* find(size_t key) const
        {
            size_t hash;
            if (FindElementHash(key, hash))
            {
                return m_elementArray[hash];
            }
            else
            {
                return nullptr;
            }
        }

        std::vector<size_t> get_keys() const
        {
            std::vector<size_t> result;
            for (int i = 0; i < m_maxCapacity; i++)
            {
                if (m_elementArray[i] != nullptr)
                {
                    result.push_back(m_elementArray[i]->key);
                }
            }
            return result;
        }
    };

    class GOAPValue
    {
        char* m_valueBuffer = nullptr;
        size_t m_size = 0;

    public:
        GOAPValue() {}

        GOAPValue(char* valueSrc, size_t valueSize)
        {
            m_valueBuffer = valueSrc;
            m_size = valueSize;
        }

        GOAPValue(void* valueSrc, size_t valueSize)
        {
            m_valueBuffer = (char*)valueSrc;
            m_size = valueSize;
        }

        ~GOAPValue()
        {

        }

        // Set the internal Value Reference of this GOAPValue
        void SetValueRef(void* valueSrc, size_t valueSize)
        {
            m_valueBuffer = (char*)valueSrc;
            m_size = valueSize;
        }

        void CopyValueRef(const GOAPValue& other)
        {
            m_valueBuffer = other.m_valueBuffer;
            m_size = other.m_size;
        }

        // Gets the current value
        template<class T>
        T& GetValue() const
        {
            return (*(T*)m_valueBuffer);
        }

    private:
        void MemCopyValue(const void* src)
        {
            std::memcpy(m_valueBuffer, src, m_size);
        }

    public:
        // Set value to the desired value
        // Uses copy assignment to set the value. This could cause memory stomping.
        // Does not care what size the goap value is.
        // This is fine to use only if Type T is the expected value that this->m_valueBuffer is pointing to.
        template<class T>
        void SetValueUnsafe(const T& value)
        {
            (*(T*)m_valueBuffer) = value;
        }

        // Set value to the desired value
        // Creates a copy of value, then memcpys the the data of the newly constructed copy using this->m_size.
        // Ensures the memory copied is only the size of the value size the goap value was initialised with.
        // If this->m_size is not the expected size of variables in Type T, then undefined behaviour could occur.
        // This could happen mostly in the case where Type T is not the expected value that this->m_valueBuffer is pointing to.
        template<class T>
        void SetValue(const T& value)
        {
            // Use copy constructor to create a new copy of the desired value.
            T temp = T(value);

            // Copy the memory values of the newly constructed copy but only of size this->m_size.
            MemCopyValue(&temp);

            // Set the copied values of the from the temp object to zero.
            // This is to prevent the destructor from removing references that the copied values depends on.
            memset(&temp, 0, m_size);
        }

        int& GetInt() const
        {
            return (*(int*)m_valueBuffer);
        }

        void SetInt(int value)
        {
            MemCopyValue(&value);
        }

        float& GetFloat() const
        {
            return (*(float*)m_valueBuffer);
        }

        void SetFloat(float value)
        {
            MemCopyValue(&value);
        }

        bool& GetBool() const
        {
            return (*(bool*)m_valueBuffer);
        }

        void SetBool(bool value)
        {
            MemCopyValue(&value);
        }
    };

    class GOAPReadValue
    {
        const char* m_valueBuffer = nullptr;

    public:
        GOAPReadValue(const char* valueSrc)
        {
            m_valueBuffer = valueSrc;
        }

        GOAPReadValue(const void* valueSrc)
        {
            m_valueBuffer = (const char*)valueSrc;
        }

        ~GOAPReadValue()
        {

        }

        // Gets the current value
        template<class T>
        const T& GetValue() const
        {
            return (*(const T*)m_valueBuffer);
        }

        int GetInt() const
        {
            return (*(const int*)m_valueBuffer);
        }

        float GetFloat() const
        {
            return (*(const float*)m_valueBuffer);
        }

        bool GetBool() const
        {
            return (*(const bool*)m_valueBuffer);
        }
    };

    // This class absolutely hates string literals.
    // I suspect it's because of how char* are stored as c strings.
    // The null terminator may be confusing the deallocation from the heap.
    // or maybe it's a lot more simpler and pointers just aren't compatiable with how I've set this up.
    class GOAPWorldState
    {
        RedirectMap m_keyIndices;
        MultiTypeArray m_worldValues;

    public:
        GOAPWorldState()
        {

        }
        ~GOAPWorldState()
        {

        }

        bool CreateValueType(int key, size_t typeSize)
        {
            if (!Contains(key))
            {
                m_keyIndices.insert(key, m_keyIndices.size());
                m_worldValues.AppendValueSize(typeSize);
                return true;
            }
            else
            {
                return false;
            }
        }

        void CreateValueTypes(int* keyArray, size_t* typeSizeArray, size_t count)
        {
            std::vector<size_t> toAddSizes;

            for (size_t i = 0; i < count; i++)
            {
                if (!Contains(keyArray[i]))
                {
                    toAddSizes.push_back(typeSizeArray[i]);
                    m_keyIndices.insert(keyArray[i], m_keyIndices.size());
                }
            }

            m_worldValues.AppendRangeValueSize(toAddSizes.begin()._Ptr, toAddSizes.size());
        }

        size_t GetValueTypeSize(int key) const
        {
            return m_worldValues.GetValueSize(m_keyIndices.find(key)->value);
        }

        void Clear()
        {
            m_keyIndices.clear();
            m_worldValues.Clear();
        }

        bool Contains(int key)
        {
            return m_keyIndices.contains(key);
        }

        template <class T>
        void SetValue(int key, const T& value)
        {
            m_worldValues.SetValue<T>(m_keyIndices[key], value);
        }

        void CopyValue(int key, char* valueBuffer)
        {
            memcpy(GetValueRef(key), valueBuffer, GetValueTypeSize(key));
        }

        void CopyConstValue(int key, const char* valueBuffer)
        {
            memcpy(GetValueRef(key), valueBuffer, GetValueTypeSize(key));
        }

        void CopyValues(const GOAPWorldState& other)
        {
            auto otherKeys = other.GetKeys();
            for (auto key : otherKeys)
            {
                CreateValueType(key, other.GetValueTypeSize(key));
                CopyValue(key, other.GetConstValueRef(key));
            }
        }

        template <class T>
        T& GetValue(int key)
        {
            return m_worldValues.GetValue<T>(m_keyIndices[key]);
        }

        char* GetValueRef(int key)
        {
            return m_worldValues.GetValuePtr(m_keyIndices[key]);
        }

        char* GetConstValueRef(int key) const
        {
            return m_worldValues.GetConstValuePtr(m_keyIndices.find(key)->value);
        }

        GOAPValue GetGOAPValue(int key)
        {
            return GOAPValue(GetValueRef(key), GetValueTypeSize(key));
        }

        GOAPReadValue GetReadValue(int key) const
        {
            return GOAPReadValue(GetConstValueRef(key));
        }

        std::vector<int> GetKeys() const
        {
            std::vector<int> keys;
            auto unsignedKeys = m_keyIndices.get_keys();

            for (int i = 0; i < unsignedKeys.size(); i++)
            {
                keys.push_back(unsignedKeys[i]);
            }
            return keys;
        }

        static void FillValues(std::vector<int> keys, const GOAPWorldState& in, GOAPWorldState& out)
        {
            for (int i = 0; i < keys.size(); i++)
            {
                auto key = keys[i];
                if (out.CreateValueType(key, in.GetValueTypeSize(key)))
                {
                    out.CopyValue(key, in.GetConstValueRef(key));
                }
            }
        }

        static void FillValues(const GOAPWorldState& in, GOAPWorldState& out)
        {
            out.CopyValues(in);
        }

        // Copy constructor
        GOAPWorldState(const GOAPWorldState& other)
        {
            CopyValues(other);
        }

        // Assignment
        GOAPWorldState& operator=(const GOAPWorldState& other)
        {
            Clear();
            CopyValues(other);
            return *this;
        }
    };

    // This is a container that holds keys to index into various GOAP variables
    // TODO:: create my own dynamic array to hold keys. Remove dependence on std::vector
    class GOAPKeyList
    {
        std::vector<int> m_keys;

    protected:
        void AddKey(int key)
        {
            if (std::count(m_keys.begin(), m_keys.end(), key))
            {
                // key already exists
            }
            else
            {
                // key does not exist
                m_keys.push_back(key);
            }
        }

        void ClearKeys()
        {
            m_keys.clear();
        }

    public:
        std::vector<int> GetKeys()
        {
            return m_keys;
        }
    };

    class GOAPAction : public GOAPKeyList
    {
    public:
        GOAPAction() {}
        ~GOAPAction() {}

        // current is the temporary world state that is action will affect. ie. the current node the planning tree is looking at.
        // base is the actual world state that the planner is looking at. ie. the root node the planner started from.
        // out is the resulting worldstae after the action has taken effect. ie. the next node of the planner.
        bool ProcessAction(GOAPWorldState& current, const GOAPWorldState& base, GOAPWorldState& out)
        {
            // fill out with currents keys and values
            GOAPWorldState::FillValues(current.GetKeys(), current, out);


            // fill out with keys and base values
            GOAPWorldState::FillValues(GetKeys(), base, out);

            // check if out meets conditions
            if (!CheckConditions(out))
            {
                return false;
            }

            // perform effects if valid
            PerformEffects(out);

            return true;
        }

        virtual void PerformEffects(GOAPWorldState& worldState) = 0;

        virtual bool CheckConditions(const GOAPWorldState& worldState) = 0;
    };

    class GOAPGoal : public GOAPKeyList
    {
    public:
        // current is the world state at the current node.
        // base is the root where the planning tree started.
        virtual bool PerformConditions(const GOAPWorldState& current, const GOAPWorldState& base) = 0;
    };

    class GOAPNode
    {
        size_t m_parentIndex;
        size_t m_depth;

        GOAPWorldState m_worldState;

        // The action that was used to get to this worldstate
        GOAPAction* m_action;

    public:
        GOAPNode(const GOAPWorldState& worldState)
        {
            m_parentIndex = -1;
            m_worldState.CopyValues(worldState);
            m_depth = 0;
            m_action = nullptr;
        }

        GOAPNode(size_t parentIndex, size_t depth, GOAPAction* action)
        {
            m_parentIndex = parentIndex;
            m_depth = depth;
            m_action = action;
        }

        GOAPNode(const GOAPNode& other)
        {
            m_parentIndex = other.m_parentIndex;
            m_depth = other.m_depth;

            GOAPWorldState::FillValues(other.m_worldState, m_worldState);
            m_action = other.m_action;
        }

        GOAPNode& operator=(const GOAPNode& other)
        {
            m_parentIndex = other.m_parentIndex;
            m_depth = other.m_depth;

            GOAPWorldState::FillValues(other.m_worldState, m_worldState);
            m_action = other.m_action;

            return *this;
        }

        size_t GetParentIndex()
        {
            return m_parentIndex;
        }

        size_t GetDepth()
        {
            return m_depth;
        }

        GOAPAction* GetAction()
        {
            return m_action;
        }

        bool ProcessAction(std::vector<GOAPNode>& nodeTree, const GOAPWorldState& base)
        {
            return m_action->ProcessAction(nodeTree[m_parentIndex].m_worldState, base, m_worldState);
        }

        bool MeetsGoal(GOAPGoal& goal, const GOAPWorldState& base)
        {
            return goal.PerformConditions(m_worldState, base);
        }
    };

    // This is intended to be a static class
    class GOAPPlanner
    {
        GOAPPlanner() {}

        static std::vector<GOAPNode>& nodeTree()
        {
            static std::vector<GOAPNode> _nodeTree;
            return _nodeTree;
        }

        static std::vector<size_t>& goalIndexList()
        {
            static std::vector<size_t> _goalIndexList;
            return _goalIndexList;
        }

    public:
        static std::stack<GOAPAction*> GetPlanStack(const GOAPWorldState& worldState, std::vector<GOAPAction*>& m_actionList, GOAPGoal& goal)
        {
            GOAPNode* goalNode = BuildTree(worldState, m_actionList, goal);

            std::stack<GOAPAction*> actionStack;
            if (goalNode != nullptr)
            {
                GOAPNode* currentNode = goalNode;
                while (currentNode->GetDepth() != 0)
                {
                    actionStack.push(currentNode->GetAction());
                    currentNode = &(nodeTree()[currentNode->GetParentIndex()]);
                }
            }
            return actionStack;
        }

        static std::queue<GOAPAction*> GetPlanQueue(const GOAPWorldState& worldState, std::vector<GOAPAction*>& m_actionList, GOAPGoal& goal)
        {
            std::stack<GOAPAction*> plan = GetPlanStack(worldState, m_actionList, goal);

            std::queue<GOAPAction*> result;
            while (!plan.empty())
            {
                result.push(plan.top());
                plan.pop();
            }

            return result;
        }

    private:
        static GOAPNode* BuildTree(const GOAPWorldState& worldState, std::vector<GOAPAction*>& m_actionList, GOAPGoal& goal)
        {
            auto _nodeTree = &nodeTree();
            auto _goalIndexList = &goalIndexList();

            // start
            _nodeTree->clear();
            _goalIndexList->clear();

            // create initial node with worldState that contains at least the goal keys
            GOAPWorldState initialState;
            GOAPWorldState::FillValues(goal.GetKeys(), worldState, initialState);
            GOAPNode initial(initialState);
            _nodeTree->push_back(initial);

            size_t currentNodeIndex = 0;
            size_t minDepth = 4294967295; // uINT_MAX

            size_t minDepthNodeIndex = 0;

            // loop all nodes in the current nodeTree
            while (currentNodeIndex != _nodeTree->size())
            {
                for (GOAPAction* action : m_actionList)
                {
                    GOAPNode node(currentNodeIndex, (*_nodeTree)[currentNodeIndex].GetDepth() + 1, action);
                    if (node.GetDepth() <= minDepth && node.GetDepth() <= MAX_DEPTH)
                    {
                        // process node
                        if (node.ProcessAction(*_nodeTree, worldState))
                        {
                            // conditions were met and effects are applied. add this node as a new leaf
                            _nodeTree->push_back(node);

                            // check if the node is results in the goal conditions
                            if (node.MeetsGoal(goal, worldState))
                            {
                                size_t goalIndex = _nodeTree->size() - 1;
                                _goalIndexList->push_back(goalIndex);
                                size_t depth = node.GetDepth();
                                if (depth < minDepth)
                                {
                                    minDepthNodeIndex = goalIndex;
                                    minDepth = node.GetDepth();
                                }
                            }
                        }
                    }
                }

                currentNodeIndex++;
            }

            return &(*_nodeTree)[minDepthNodeIndex];
        }

    public:
        static const std::vector<GOAPNode>* GetLastTree()
        {
            return &nodeTree();
        }
    };
}

#endif // -ends GOAPCORE_H