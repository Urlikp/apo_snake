    class Rectangle{
        public:
            int x, y, w, h;
            Rectangle(int startX, int startY, int width, int height){
                x = startX;
                y = startY;
                w = width;
                h = height;
            }
            bool intersects(Rectangle *rect){
                if (x > rect->x + rect->w || rect->x > x + w) 	{
                    return false;
                }
                if (y > rect->y + rect->h || rect->y > y + h) 	{
                    return false;
                }
                return true; 
            }
    };