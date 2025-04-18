#include <unistd.h>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <fstream>
#include <iostream>

// VTK 헤더 파일
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>

using namespace std;
using namespace Eigen;

string trajectory_file = "./trajectory.txt";

void DrawTrajectory(vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>>);

int main(int args, char** argv) {
  vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>> poses;
  ifstream fin(trajectory_file);
  if (!fin) {
    cout << "cannot find trajectory file at " << trajectory_file << endl;
    return 1;
  }

  while (!fin.eof()) {
    double time, tx, ty, tz, qx, qy, qz, qw;
    fin >> time >> tx >> ty >> tz >> qx >> qy >> qz >> qw;
    Isometry3d Twr(Quaterniond(qw, qx, qy, qz));
    Twr.pretranslate(Vector3d(tx, ty, tz));
    poses.push_back(Twr);
  }

  DrawTrajectory(poses);
  return 0;
}

void DrawTrajectory(
    vector<Isometry3d, Eigen::aligned_allocator<Isometry3d>> poses) {
  // 렌더러, 렌더 윈도우 및 인터랙터 생성
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  renderer->SetBackground(1.0, 1.0, 1.0);  // 배경색 흰색

  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(1024, 768);
  renderWindow->SetWindowName("Trajectory Viewer");

  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // 포즈 데이터를 저장할 포인트 및 라인 생성
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();

  // 각 포즈별 점 좌표를 추가
  for (size_t i = 0; i < poses.size(); i++) {
    Eigen::Vector3d position = poses[i].translation();
    points->InsertNextPoint(position[0], position[1], position[2]);

    // 라인 추가 (첫 번째 점 제외)
    if (i > 0) {
      vtkSmartPointer<vtkLine> line = vtkSmartPointer<vtkLine>::New();
      line->GetPointIds()->SetId(0, i - 1);
      line->GetPointIds()->SetId(1, i);
      lines->InsertNextCell(line);
    }
  }

  // 포인트와 라인을 합쳐서 폴리데이터 생성
  vtkSmartPointer<vtkPolyData> linesPolyData =
      vtkSmartPointer<vtkPolyData>::New();
  linesPolyData->SetPoints(points);
  linesPolyData->SetLines(lines);

  // 라인을 렌더링하기 위한 매퍼와 액터 생성
  vtkSmartPointer<vtkPolyDataMapper> lineMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  lineMapper->SetInputData(linesPolyData);

  vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
  lineActor->SetMapper(lineMapper);
  lineActor->GetProperty()->SetColor(0.0, 0.0, 0.0);  // 검은색 선
  lineActor->GetProperty()->SetLineWidth(2.0);

  renderer->AddActor(lineActor);

  // 각 포즈마다 좌표축 표시
  for (size_t i = 0; i < poses.size(); i++) {
    vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();

    // 좌표축 크기 설정
    axes->SetTotalLength(0.1, 0.1, 0.1);
    axes->SetShaftTypeToCylinder();
    axes->SetCylinderRadius(0.01);

    // 좌표축 위치 및 방향 설정을 위한 변환 행렬
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();

    // Eigen::Isometry3d를 VTK 변환 행렬로 변환
    Eigen::Matrix4d eigenMat = poses[i].matrix();
    double vtkMat[16];
    for (int r = 0; r < 4; r++) {
      for (int c = 0; c < 4; c++) {
        vtkMat[r * 4 + c] = eigenMat(r, c);
      }
    }

    transform->SetMatrix(vtkMat);
    axes->SetUserTransform(transform);

    renderer->AddActor(axes);
  }

  // 카메라 위치 설정
  renderer->GetActiveCamera()->SetPosition(0, -0.1, -1.8);
  renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
  renderer->GetActiveCamera()->SetViewUp(0.0, -1.0, 0.0);

  // 렌더링 시작
  renderWindow->Render();
  renderWindowInteractor->Start();
}