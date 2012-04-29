// Based on http://www.boost.org/doc/libs/1_49_0/doc/html/boost_asio/tutorial/tuttimer5/src.html

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace ba = boost::asio;

namespace
{
    class Printer
    {
    private:
        ba::strand strand_;
        int count_;

        void print(const char* msg)
        {
            std::cout << boost::this_thread::get_id() << ' ' << msg << ' ' << count_ << std::endl;
        }

    public:
        Printer(ba::io_service& aios, int count) : strand_(aios), count_(count)
        {
            strand_.post(std::bind(&Printer::print1, this));
            strand_.post(std::bind(&Printer::print2, this));
        }

        ~Printer()
        {
            print("dtor");
        }

        void print1()
        {
            if(count_ > 0)
            {
                print("print one");
                --count_;
                strand_.post(std::bind(&Printer::print1, this));
            }
        }

        void print2()
        {
            if(count_ > 0)
            {
                print("print two");
                --count_;
                strand_.post(std::bind(&Printer::print2, this));
            }
        }
    };
}

void strand()
{
    ba::io_service aios;
    Printer p(aios, 10);
    boost::thread t(std::bind(&ba::io_service::run, &aios));
    aios.run();
    t.join();
}
