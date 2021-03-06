/**
 * Created by zhuyl on 2016/11/14.
 */

DaQOCloud.controller('echarsOperationModelOneDataDlgCtrl',['$scope','$filter','$modalInstance','paramObj',function($scope,$filter,$modalInstance,paramObj) {


    $scope.modelDlg = {
        checkTitle:false,
        checkY:false,
        checkTime:false
    };

    $scope.selectEcharsData = paramObj.selectData;
    if($scope.selectEcharsData.param != null){
        $scope.selectEcharsData.lstParams = [];
        $scope.selectEcharsData.lstParams.push($scope.selectEcharsData.param)
    }
    $scope.lstDeviceInfo = paramObj.lstDeviceInfo;
    $scope.selectDeviceInfo =null;
    $scope.selectPoint =null;
    $scope.lstDBParams =null;

    $(function() {

        getCheckInfo();

    });


    function  getCheckInfo() {

        if ($scope.selectEcharsData.strChartKind =="ecLine"){
            $scope.modelDlg.checkTitle =true;
            $scope.modelDlg.checkY = true;
            $scope.modelDlg.checkTime =true;

        }
        if ($scope.selectEcharsData.strChartKind =="ecBar"){
            $scope.modelDlg.checkTitle =true;
            $scope.modelDlg.checkY = false;
            $scope.modelDlg.checkTime =false;

        }
        if ($scope.selectEcharsData.strChartKind =="ecPie"){
            $scope.modelDlg.checkTitle =true;
            $scope.modelDlg.checkY = false;
            $scope.modelDlg.checkTime =false;
        }
        if ($scope.selectEcharsData.strChartKind =="ecYibiaopan"){
            $scope.modelDlg.checkTitle =false;
            $scope.modelDlg.checkY = true;
            $scope.modelDlg.checkTime =false;
        }
        if ($scope.selectEcharsData.strChartKind =="dataText"){
            $scope.modelDlg.checkTitle =false;
            $scope.modelDlg.checkY = false;
            $scope.modelDlg.checkTime =false;

        }
        if ($scope.selectEcharsData.strChartKind =="switch"){
            $scope.modelDlg.checkTitle =false;
            $scope.modelDlg.checkY = false;
            $scope.modelDlg.checkTime =false;
        }


    }

    $scope.ok = function(){

        $modalInstance.close($scope.selectEcharsData); //关闭并返回当前选项
    };
    $scope.cancel = function(){
        $modalInstance.dismiss('cancel'); // 退出
    };

    $scope.getCheckPoint = function (onedevice) {
        $scope.selectDeviceInfo  = onedevice;
        $scope.lstDBParams = onedevice.lstDBParams;
    };

    $scope.selectCheckPoint = function (onecheckpoint) {

        $scope.selectPoint = onecheckpoint;
    };



    $scope.addPoint = function () {
        if ($scope.selectDeviceInfo ==null||$scope.selectPoint == null){
            return ;
        }

        if ($scope.selectEcharsData.param != null){
            alert("只能绑定一个数据");
            return;
        }
        var chartOneParamInfo = new ChartOneParamInfo();
        chartOneParamInfo.strDBValue = 50;
        chartOneParamInfo.strDBKey = $scope.selectPoint.strDBKey;
        chartOneParamInfo.strTitle = $scope.selectPoint.strTitle;
        chartOneParamInfo.intDeviceId = $scope.selectDeviceInfo.intDeviceid;
        chartOneParamInfo.strDeviceName =$scope.selectDeviceInfo.strDeviceName;
        $scope.selectEcharsData.lstParams = [];
        $scope.selectEcharsData.param = chartOneParamInfo;
        $scope.selectEcharsData.lstParams.push(chartOneParamInfo);

    };

    $scope.deletePoint = function (target) {
        $scope.selectEcharsData.param = null;
        $scope.selectEcharsData.lstParams.pop(target);

    };
}]);

