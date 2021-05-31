# MFC_bmp_background_com

### bmp 이미지 제작 환경 구축
#### - OnOpenDocument : bmp 파일의 헤더 파일과 데이터 구조를 확인하고 파일 오픈
#### - OnGrayScale : bmp 파일 컬러를 흑백으로 변경
#### - OnDownSampling : bmp 파일 다운 샘플링 후 헤더파일 컨트롤
#### - OnSaveDocument : bmp 파일 변경된 이미지 파일과 변경된 헤더파일 저장
#### - OnBackground : 선택된 이미지와 배경 이미지를 이용하여 자동 배경 합성


### 자동 마스크 생성에 사용될 모델
#### - jupyter notebook 환경에서 제작
#### - https://www.kaggle.com/kairess/human-segmentation 에서 이미지와 이미지 마스크 데이터 이용
#### - CNN ResNet 아키텍쳐 이용하여 모델 학습
