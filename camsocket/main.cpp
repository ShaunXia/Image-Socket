#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace boost;
using boost::asio::ip::tcp;
using namespace std;
using namespace cv;


int main()
{
    char buff[1024];
    char *finbuff=new char[777640];
    size_t len=0;
    asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 6000));
    int count=0;
    while(1)
    {
        char t[256];
        len=0;
        tcp::socket socket(io_service);
        acceptor.accept(socket);
        std::cout << socket.remote_endpoint().address() << std::endl;
        try
        {
            int offset = 0;
            for (;;)
            {
                boost::system::error_code error;
                len= socket.read_some(boost::asio::buffer(buff));
                memcpy(finbuff + offset,buff,len);
                offset += len;
                if (error == boost::asio::error::eof)
                    break;
                // Connection closed cleanly by peer.
                else if (error)
                    throw error;
                // Some other error.
                cout<<len<<endl;
            }
        }
        catch (std::exception& e)
        {
            CvMat mat = cvMat(960,540,CV_8UC1, finbuff);
            IplImage *pp = cvDecodeImage( &mat, 1 );
            cout<<pp->width<<" "<<pp->height<<endl;
            cvNamedWindow("a");
            cvShowImage("a",pp);
            cvWaitKey(33);
            std::cerr << e.what() << std::endl;
            cvReleaseImage(&pp);
        }
        ++count;
    }
    
    return 0;
}