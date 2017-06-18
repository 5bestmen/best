/**
 * Created by zhuyl on 2016/11/14.
 */

DaQOCloud.controller('echarsOperationModelDlgCtrl',['$scope', '$filter','$modalInstance','paramObj',function($scope,$filter,$modalInstance,paramObj) {


    $scope.modelDlg = {
        checkTitle:false,
        checkY:false,
        checkTime:false
    };

    $scope.selectEcharsData = paramObj.selectData;
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
        if ($scope.selectEcharsData.strChartKind =="ecArealine"){
            $scope.modelDlg.checkTitle =true;
            $scope.modelDlg.checkY = true;
            $scope.modelDlg.checkTime =true;

        }
        if ($scope.selectEcharsData.strChartKind =="ecBar"){
            $scope.modelDlg.checkTitle =true;
            $scope.modelDlg.checkY = true;
            $scope.modelDlg.checkTime =false;

        }
        if ($scope.selectEcharsData.strChartKind =="ecPie"){
            $scope.modelDlg.checkTitle =true;
            $scope.modelDlg.checkY = true;
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
        var charTendParamInfo = new ChartTendParamInfo();
        for(var i=0;i<$scope.selectEcharsData.intGapNumber;i++){
            charTendParamInfo.lstDBvalue.push(50);
        }
        charTendParamInfo.strDBKey = $scope.selectPoint.strDBKey;
        charTendParamInfo.strTitle = $scope.selectPoint.strTitle;
        charTendParamInfo.intDeviceId = $scope.selectDeviceInfo.intDeviceid;
        charTendParamInfo.strDeviceName =$scope.selectDeviceInfo.strDeviceName;
        $scope.selectEcharsData.lstParams.push(charTendParamInfo);
    };

    $scope.deletePoint = function (onecheckpoint) {
        $scope.selectEcharsData.lstParams.pop(onecheckpoint);
    };


}]);

