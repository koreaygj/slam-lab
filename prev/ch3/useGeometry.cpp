#include <cmath>
#include <iostream>
using namespace std;

#include <Eigen/Core>
#include <Eigen/Geometry>
using namespace Eigen;

int main(int args, char **argv) {
  // 3 * 3 matrix
  Matrix3d rotation_matrix = Matrix3d::Identity();
  // 각도: pi / 4, 회전축: (0, 0, 1)로 생성
  // 1번째 열 회전후 x 축 방향
  // 2번째 열 회전후 y 축 방향
  // 3번째 열 회전후 z 축 방향
  AngleAxisd rotation_vector(M_PI / 4, Vector3d(0, 0, 1));
  cout.precision(3);
  cout << "rotation matrix = \n " << rotation_vector.matrix() << endl;
  // 직접적으로 매트릭스 설정 가능
  rotation_matrix = rotation_vector.toRotationMatrix();

  // angleAxis 활용해서 회전적용
  Vector3d v(1, 0, 0);
  Vector3d v_rotated = rotation_vector * v;
  cout << "(1, 0, 0) after rotation (by angle axis) = " << v_rotated.transpose()
       << endl;
  // rotation_matrix 활용해서 회전 적용
  v_rotated = rotation_matrix * v;
  cout << "(1, 0, 0) after rotation (by matrix) = " << v_rotated.transpose()
       << endl;

  // Euler angle 활용해서 적용
  // 회전행렬을 zyx 순서로 적용 (2, 1, 0)
  // z: yaw, y: pitch, x: roll
  Vector3d euler_angles = rotation_matrix.eulerAngles(2, 1, 0);
  cout << "yaw pitch roll = " << euler_angles.transpose() << endl;

  // Euclidean transformation matrix using Eigen::Isometry
  Isometry3d T = Isometry3d::Identity();
  // T에 회전변환 적용
  T.rotate(rotation_vector);
  // T에 이동 변환 적용
  T.pretranslate(Vector3d(1, 3, 4));
  cout << "Transform matrix = \n" << T.matrix() << endl;
  // v에 T 변환 적용
  Vector3d v_transformed = T * v;
  cout << "v transformed = " << v_transformed.transpose() << endl;

  // Quaternion 사용해서 3D 회전하기
  Quaterniond q = Quaterniond(rotation_vector);
  // rotation vector quaternion 적용
  // 쿼터니안 계수 i, j, k
  // coeffs()를 통해서 백터 형태로 반환, transpose()를 통해서 행 백터로 전환
  cout << "quaternion from rotation vector = " << q.coeffs().transpose()
       << endl;
  // rotation matrix quaternion 적용
  // coeffs()를 통해서 백터 형태로 반환, transpose()를 통해서 행 백터로 전환
  q = Quaterniond(rotation_matrix);
  cout << "quaternion from rotation matrix = " << q.coeffs().transpose()
       << endl;
  // Quaternion을 통해 백터 회전
  v_rotated = q * v;
  cout << "(1, 0, 0) after rotation = " << v_rotated.transpose() << endl;
  cout << "should be equal to "
       << (q * Quaterniond(0, 1, 0, 0) * q.inverse()).coeffs().transpose()
       << endl;
  return 0;
}