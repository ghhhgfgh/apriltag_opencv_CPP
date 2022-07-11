#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

extern "C" {
	#include<apriltag/apriltag.h>
	#include<apriltag/tag36h11.h>
	#include<apriltag/common/image_u8.h>
	#include<apriltag/common/zarray.h>
	#include<apriltag/apriltag_pose.h>
}

using namespace std;
int main(int argc, char **argv) {

  cv::Mat image;
  image = cv::imread(argv[1]); 


  if (image.data == nullptr) { 
    cerr << "文件" << argv[1] << "不存在." << endl;
    return 0;
  }

  cout << "图像宽为" << image.cols << ",高为" << image.rows << ",通道数为" << image.channels() << endl;
  cv::imshow("image", image);      
  cv::waitKey(0);               


  if (image.type() != CV_8UC1 && image.type() != CV_8UC3) {
    cout << "请输入一张彩色图或灰度图." << endl;
    return 0;
  }

  image_u8_t img_header = {
      .width = image.cols,
      .height = image.rows,
      .stride = image.cols,
      .buf = image.data
  };

  apriltag_detector_t *td = apriltag_detector_create();
  apriltag_family_t *tf = tag36h11_create();
  apriltag_detector_add_family(td, tf);
  zarray_t *detections = apriltag_detector_detect(td, img_header);

  for (int i = 0; i < zarray_size(detections); i++) 
  {
      apriltag_detection_t *det;
/* The defination of apriltag_detection_t
      # c[2]: The center of the detection in image pixel coordinates.
      double 	c [2]

      # decision_margin: A measure of the quality of the binary decoding process
      float 	decision_margin

      apriltag_family_t * 	family

      # goodness: A measure of the quality of tag localization
      float 	goodness

      # H:
     // The 3x3 homography matrix describing the projection from an
     // "ideal" tag (with corners at (-1,-1), (1,-1), (1,1), and (-1,
     // 1)) to pixels in the image. This matrix will be freed by
     // apriltag_detection_destroy.
      matd_t * 	H

      # hamming:
      // How many error bits were corrected? Note: accepting large numbers of
      // corrected errors leads to greatly increased false positive rates.
      // NOTE: As of this implementation, the detector cannot detect tags with
      // a hamming distance greater than 2.
      int 	hamming

      # id
      // The decoded ID of the tag
      int 	id

      #p
      // The corners of the tag in image pixel coordinates. These always
      // wrap counter-clock wise around the tag.
      double 	p [4][2]
*/

      zarray_get(detections, i, &det);
      cout << "The " << i << " TAG" <<std::endl;
      cout << "The center " << det->c[0] << " ," << det->c[1] <<std::endl;
      cout << "The goodness " << det->goodness <<std::endl;
      cout << "The hamming " << det->hamming <<std::endl;
      cout << "The id " << det->id <<std::endl;
      for(int nu = 0;nu <4;nu++){
      cout << "The " << nu <<" concer pixel point " << det->p[nu][0]<<" , " <<<det->p[nu][1]<std::endl;
      }

  }

  tag36h11_destroy(tf);
  apriltag_detector_destroy(td);

  cv::imshow("image", image);
  cv::waitKey(0);

  cv::destroyAllWindows();
  return 0;
}
