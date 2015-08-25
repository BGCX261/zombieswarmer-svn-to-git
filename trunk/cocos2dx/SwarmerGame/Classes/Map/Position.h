#ifndef _POSITION_H_
#define _POSITION_H_

class Position
{
public:
   Position()
      : m_x(0xFFFFFFFF)
      , m_y(0xFFFFFFFF)
   {/*NO IMPL*/}

   Position(unsigned int x, unsigned int y)
      : m_x(x)
      , m_y(y)
   {/*NO IMPL*/}

   unsigned int x() const {return m_x;}
   unsigned int y() const {return m_y;}

   void setPos(unsigned int x, unsigned int y) {m_x = x; m_y = y; }
   void setX(unsigned int x) {m_x = x;}
   void setY(unsigned int y) {m_y = y;}

   bool operator ==(const Position p) const { return (p.x() == x() && p.y() == y() ); }
   bool operator!=(const Position p) const { return !(*this == p); }

   static bool inRange(const Position& p1, const Position& p2, unsigned int range)
   {
      //TODO: need to be changed to take diagonals into consideration 
      return (static_cast<unsigned int>(std::abs(static_cast<int>(p2.x() - p1.x()))) <= range) && (static_cast<unsigned int>(std::abs(static_cast<int>(p1.y() - p2.y()))) <= range);
      //double x = std::abs(static_cast<int>(p1.x() - p2.x()));
      //double y = std::abs(static_cast<int>(p1.y() - p2.y()));
      //return std::floor(std::sqrt(x + y)) <= static_cast<double>(range);
   }

   bool isValid() {return m_x != 0xFFFFFFFF && m_y != 0xFFFFFFFF; }

private:
   unsigned int m_x;
   unsigned int m_y;
};

////////////////////////////////////////////////////////////////////////////////

template <class T>
class Rect
{
public:
   Rect()
      : m_x(0)
      , m_y(0)
      , m_width(0)
      , m_height(0)
   {
   }

   Rect(const Rect& src)
      : m_x(src.m_x)
      , m_y(src.m_y)
      , m_width(src.m_width)
      , m_height(src.m_height)
   {
   }

   Rect(T x, T y, T width, T height)
      : m_x(x)
      , m_y(y)
      , m_width(width)
      , m_height(height)
   {
   }

   Rect& operator =(const Rect& src)
   {
      m_x = src.m_x;
      m_y = src.m_y;
      m_width = src.m_width;
      m_height = src.m_height; 

      return *this;
   }

   bool isEmpty()
   {
      return ( !m_width && !m_height );
   }

   Position center()
   {
      return Position( m_x + m_width/2, m_y + m_height/2 );
   }

   bool pointInRect(Position p) const
   {
      return ( ( (p.x() >= m_x) && (p.x() <= m_x + m_width)) &&
               ( (p.y() >= m_y) && (p.y() <= m_y + m_height)) );
   }

   T x()                      { return m_x; }
   T y()                      { return m_y; }
   T width()                  { return m_width; }
   T height()                 { return m_height; }

private:

   T        m_x, m_y;
   T        m_width, m_height;
};

#endif //_POSITION_H_