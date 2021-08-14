#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;
namespace po = boost::program_options;

int main(int argc, char **argv)
{
    int matchMethod;
    float score_threshold;
    float nms_threshold;
    string src_name;
    string temp_name;
    string output_name;

    po::options_description desc("Template matching with Non-Maximum Suppression\nAllowed options");

    desc.add_options()("help", "Print help message");
    desc.add_options()("source", po::value<string>(&src_name),
                       "score image");
    desc.add_options()("template", po::value<string>(&temp_name),
                       "template image");
    desc.add_options()("output", po::value<string>(&temp_name),
                       "output image");
    desc.add_options()("score", po::value<float>(&score_threshold)->default_value(0.7f),
                       "score threshold");
    desc.add_options()("nms", po::value<float>(&nms_threshold)->default_value(0.3f),
                       "nms threshold");
    desc.add_options()("method", po::value<int>(&matchMethod)->default_value(TM_CCOEFF_NORMED),
                       "template matching Method:\n\
                       TM_SQDIFF = 0\n\
                       TM_SQDIFF_NORMED = 1\n\
                       TM_CCORR = 2\n\
                       TM_CCORR_NORMED = 3\n\
                       TM_CCOEFF = 4\n\
                       TM_CCOEFF_NORMED = 5");

    // parse command line arguementss
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        cout << desc << endl;
        return EXIT_SUCCESS;
    }

    if (vm.count("source"))
    {
        cout << "Source image:"
             << vm["source"].as<string>() << endl;
    }
    else
    {
        cout << "Source image was not set" << endl;
        return EXIT_FAILURE;
    }

    if (vm.count("template"))
    {
        cout << "Template image:"
             << vm["template"].as<string>() << endl;
    }
    else
    {
        cout << "Template image was not set" << endl;
        return EXIT_FAILURE;
    }

    if (vm.count("output"))
    {
        cout << "Output image:"
             << vm["output"].as<string>() << endl;
    }
    else
    {
        cout << "Output image was not set" << endl;
        return EXIT_FAILURE;
    }

    // read images

    Mat src = imread(src_name);
    Mat tem = imread(temp_name, 0);
    Mat gray = Mat::zeros(src.size(), CV_8UC1);
    cvtColor(src, gray, COLOR_BGR2GRAY);

    Mat resultImage;

    int resultImage_cols = gray.cols - tem.cols + 1;
    int resultImage_rows = gray.rows - tem.rows + 1;
    resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);

    // do template matching
    matchTemplate(gray, tem, resultImage, matchMethod);

    // scan for candidates boxes
    vector<Rect> bboxes;
    vector<float> scores;

    for (int i = 0; i < resultImage.rows; i++)
    {
        for (int j = 0; j < resultImage.cols; j++)
        {
            if (resultImage.at<float>(i, j) > score_threshold && resultImage.at<float>(i, j) < 1)
            {
                bboxes.push_back(Rect2d(j, i, tem.cols, tem.rows));
                scores.push_back(resultImage.at<float>(i, j));
            }
        }
    }

    // do NMS
    vector<int> filtered_idxs;
    vector<Point2d> result; // storage for centers of boxes

    cv::dnn::NMSBoxes(bboxes, scores, score_threshold, nms_threshold, filtered_idxs);

    for (auto i = filtered_idxs.begin(); i != filtered_idxs.end(); ++i)
    {
        // rectangle(src, bboxes.at(*i), Scalar(0, 0, 255), 1, 8, 0);
        Point2d center = Point2d(bboxes.at(*i).tl().x + tem.cols / 2, bboxes.at(*i).tl().y + tem.rows / 2);
        line(src, center, Point2d(center.x - tem.cols / 2, center.y), Scalar(0, 0, 255));
        line(src, center, Point2d(center.x + tem.cols / 2, center.y), Scalar(0, 0, 255));
        line(src, center, Point2d(center.x, center.y + tem.rows / 2), Scalar(0, 0, 255));
        line(src, center, Point2d(center.x, center.y - tem.rows / 2), Scalar(0, 0, 255));
        result.push_back(center);
    }

    // print result

    for (int i = 0; i < result.size(); ++i)
    {
        cout << result.at(i) << endl;
    }

    cout << "Mark number: " << filtered_idxs.size() << endl;
    imwrite(output_name, src);

    return EXIT_SUCCESS;
}