#include <iostream>
using namespace std;

#include <Eigen/Core>
#include <Eigen/Dense>
#include <ctime>
using namespace Eigen;

#define MATRIX_SIZE 50

void eigenMatrixExample(int args, char **argv) {
  // 2 * 3 행렬 생성
  Matrix<float, 2, 3> matrix_23;
  // vector -> 한개의 열 또는 행으로만 이루어진 배열
  // vector3d 는 정확히 3개의 요소를 가지고 있는 백터 ex) [2, 5, -2]
  // 기본적으로 edigen에서는 vector는 3 * 1 이런식으로 구성된다.
  Vector3d v_3d;
  Matrix<float, 3, 1> vd_3d;
  // matrix3d는 3 * 3의 정방행렬을 생성, d는 type을 의미함 d: double f: float
  Matrix3d matrix_33 = Matrix3d::Zero();
  // dynamic은 동적할당으로 런타임에 사이즈가 결정될 수 있도록 함
  Matrix<double, Dynamic, Dynamic> matrix_dynamic;
  // dynamic을 좀 더 간단하게 생성 할 수 잇음
  MatrixXd matrix_x;

  // matrix에 값을 넣는 방식
  matrix_23 << 1, 2, 3, 4, 5, 6;

  // 행렬 출력 방식
  cout << "matrix 23 print: " << matrix_23 << endl;
  // 행렬의 요소에 접근시에는 (i, j)방식으로 접근 가능
  cout << "print by for" << endl;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) cout << matrix_23(i, j) << "\t";
    cout << endl;
  }

  // vector에 넣는 방식
  v_3d << 3, 2, 1;
  vd_3d << 4, 5, 6;

  // matrix의 타입을 캐스트후 백터곱
  // transpose는 선형대수에서의 T(행과 열을 바꾸기)
  Matrix<double, 2, 1> result = matrix_23.cast<double>() * v_3d;
  cout << "[1,2,3;4,5,6]*[3,2,1]=" << result.transpose() << endl;
  Matrix<float, 2, 1> result2 = matrix_23 * vd_3d;
  cout << "[1,2,3;4,5,6]*[3,2,1]=" << result2.transpose() << endl;

  matrix_33 = Matrix3d::Random();  // Random Number Matrix
  cout << "random matrix: \n" << matrix_33 << endl;
  cout << "transpose: \n" << matrix_33.transpose() << endl;
  cout << "sum: " << matrix_33.sum() << endl;
  // trace 대각합
  cout << "trace: " << matrix_33.trace() << endl;
  cout << "times 10: \n" << 10 * matrix_33 << endl;
  // 역 행렬 A와 곱해서 단위행렬이 되는 행렬 A^(-1)을 구하는 계산
  cout << "inverse: \n" << matrix_33.inverse() << endl;
  cout << "det: " << matrix_33.determinant() << endl;

  // Eigenvalues 계산
  // Real symmetric matrix는 성공적인 대각화(diagonalization)를 보장함
  // matrix_33.transpose() * matrix_33는 항상 대칭 행렬이 됨
  SelfAdjointEigenSolver<Matrix3d> eigen_solver(matrix_33.transpose() *
                                                matrix_33);
  cout << "고유값(Eigenvalues) = \n"
       << eigen_solver.eigenvalues() << endl
       << endl;
  cout << "고유벡터(Eigenvectors) = \n"
       << eigen_solver.eigenvectors() << endl
       << endl;

  // 방정식 풀이
  // matrix_NN * x = v_Nd 형태의 방정식을 푸는 코드
  // N의 크기는 이전 매크로에서 정의되며, 랜덤 숫자로 생성됨
  // 직접 역행렬을 구하는 방법은 가장 직관적이지만, 연산량이 많음

  // 랜덤 행렬 생성
  MatrixXd matrix_NN = MatrixXd::Random(MATRIX_SIZE, MATRIX_SIZE);
  // 행렬을 양의 준정부호(semi-positive definite)로 만들기 위해 A*A^T 형태로
  // 변환
  matrix_NN = matrix_NN * matrix_NN.transpose();
  // 랜덤 벡터 생성
  MatrixXd v_Nd = MatrixXd::Random(MATRIX_SIZE, 1);

  // 시간 측정 시작
  clock_t time_stt = clock();

  // 방법 1: 직접 역행렬 계산 (Direct inversion)
  MatrixXd x = matrix_NN.inverse() * v_Nd;
  cout << "직접 역행렬 계산 방법의 소요 시간: "
       << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;
  cout << "x = " << x.transpose() << endl;

  // 방법 2: 행렬 분해 방법 (QR 분해)
  // 일반적으로 행렬 분해를 사용하면 속도가 훨씬 빠름
  time_stt = clock();
  x = matrix_NN.colPivHouseholderQr().solve(v_Nd);
  cout << "QR 분해 방법의 소요 시간: "
       << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;
  cout << "x = " << x.transpose() << endl;

  // 방법 3: 양의 정부호 행렬(positive definite matrices)의 경우
  // 콜레스키 분해(LDLT 분해)를 사용할 수 있음
  time_stt = clock();
  x = matrix_NN.ldlt().solve(v_Nd);
  cout << "LDLT 분해 방법의 소요 시간: "
       << 1000 * (clock() - time_stt) / (double)CLOCKS_PER_SEC << "ms" << endl;
  cout << "x = " << x.transpose() << endl;
}