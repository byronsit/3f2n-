//
// Created by udi on 22-6-15.
//
//通过exr格式读取数据。不同于bin格式，
//exr格式的优点是保存的是float格式，缺点是需要依赖第三方库。
//主要是二进制格式比较难定义


#include "3f2n+/3f2n+.h"
#include <exrio/exrio_opencv.h> //为了读取exr格式用的函数,对应的github为https://github.com/byronsit/exrio，下载编译安装即可
#include <opencv2/rgbd.hpp>


/**
 * @brief 可视化用的*/
void ShowNormal(std::string win, cv::Mat result) {
  cv::Mat output(result.rows, result.cols, CV_16UC3);
  for (int i = 0; i < result.rows; ++i) {
    for (int j = 0; j < result.cols; ++j) {
      result.at<cv::Vec3f>(i, j) =
          result.at<cv::Vec3f>(i, j) / cv::norm(result.at<cv::Vec3f>(i, j));
     output.at<cv::Vec3w>(i, j)[2] =
          (result.at<cv::Vec3f>(i, j)[0] + 1) * (65535 / 2.0);
      output.at<cv::Vec3w>(i, j)[1] =
          (result.at<cv::Vec3f>(i, j)[1] + 1) * (65535 / 2.0);
      output.at<cv::Vec3w>(i, j)[0] =
          (result.at<cv::Vec3f>(i, j)[2] + 1) * (65535 / 2.0);
    }
  }
  cv::imshow(win.c_str(), output);
  cv::waitKey(-1);
}


std::string path = "/home/udi/Desktop/normal/dataset/outdoor/shanghai/standard_exr/0001.exr";


int main(){
  std::vector<cv::Mat> image;
  std::vector<std::string> channel_names;
  cv::Mat depth_image, range_image;
  cv::Mat gtx, gty, gtz, tmp;
  readexr_opencv(path, image, channel_names);
  for (int i = 0; i < channel_names.size(); ++i) {
    if (channel_names.at(i) == "Z") {
      depth_image = image.at(i);
    }

    if (channel_names.at(i) == "R") {
      gtx = image.at(i);
    }

    if (channel_names.at(i) == "G") {
      gty = image.at(i);
    }

    if (channel_names.at(i) == "B") {
      gtz = image.at(i);
    }
  }
  cv::merge(std::vector<cv::Mat>{gtx, gty, gtz}, tmp);
  //if (need_show_gt) ShowNormal("gt", tmp);

  cv::Matx33f K(281.6000, 0.0, 256.0,
      0.0, 414.5777, 212.0,
      0.0, 0.0, 1.0);
  auto KK = K;

  //考虑到计算的问题，减1是必须的
  if (1) {
    KK(0, 2)--;
    KK(1, 2)--;
  }

  if (depth_image.type() != CV_32FC1) {
    depth_image.convertTo(depth_image, depth_image.rows, depth_image.cols,
                          CV_32FC1);
  }
  TFTN_Plus sne(depth_image.rows, depth_image.cols, CV_32FC1, K, 3);
  cv::Mat result;
  std::vector<cv::Mat> matpart(3);
  result.create(matpart[0].rows, matpart[0].cols, CV_32FC3);


  cv::rgbd::depthTo3d(depth_image, KK, range_image);
  sne.Work(range_image, result);

  return 0;
}
