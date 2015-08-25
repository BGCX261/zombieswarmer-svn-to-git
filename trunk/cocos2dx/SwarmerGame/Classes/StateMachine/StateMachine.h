#ifndef _STATE_MACHINE_H__
#define _STATE_MACHINE_H__

#include <string>
#include <map>

class TFunctor
{
public:
   virtual bool operator()() = 0;
};

class TransitionTrueOperation: public TFunctor
{
public:
   virtual bool operator()() { return true; }
};

template<typename T>
class TransitionOperation: public TFunctor
{
public:
   typedef bool (T::*Fp)();
   TransitionOperation(T* obj, Fp fp = NULL) : m_fp(fp), m_object(obj) {}
   virtual bool operator()() 
   {
      if (m_object && m_fp)
          return (*m_object.*m_fp)();
      return true;
   }
private:
   T* m_object;
   Fp m_fp;
};

class StateBase
{
public:
   virtual ~StateBase() {}

   virtual void onEntry() {}
   virtual void onExit()  {}
   virtual void execute(float timeSlice) = 0;
};

class StateMachine
{
public:
   StateMachine(void) {}
   virtual ~StateMachine(void) {}

   inline bool transition(const std::string& state) {
      FuncMap& fm = m_transMap[m_currentState];
      FuncMap::iterator i = fm.find(state);
      if (i != fm.end() && (*(i->second))()) {
            m_currentState = state;
            return true;
      }
      return false;
   }

   std::shared_ptr<StateBase> operator()() { return m_stateMap[m_currentState]; }
   inline const std::string& state() const { return m_currentState; }

protected:

   void setState(const std::string &name)
   {
      if (m_stateMap.find(name) != m_stateMap.end()) 
         m_currentState = name;
   }

   void setTransition(const std::string& from, const std::string& to, TFunctor* func = NULL)
   {
      if (m_stateMap.find(from) != m_stateMap.end() && m_stateMap.find(to) != m_stateMap.end()) {
         if (func == NULL) func = new TransitionTrueOperation();
         m_transMap[from][to] = std::shared_ptr<TFunctor>(func);
      };
   }

   void createState(const std::string& name, StateBase* state) 
   {
      if (m_stateMap.find(name) == m_stateMap.end())   m_stateMap[name] = std::shared_ptr<StateBase>(state);
   }



private:
   typedef std::map<std::string, std::shared_ptr<StateBase> > StateMap;
   typedef std::map<std::string, std::shared_ptr<TFunctor> >  FuncMap; 
   typedef std::map<std::string, FuncMap>                     TransitionMap;

   std::string      m_currentState;
   StateMap         m_stateMap;
   TransitionMap    m_transMap;
};

#endif // _STATE_MACHINE_H__