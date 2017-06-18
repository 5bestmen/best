/**
 * Created by reed on 2017/1/23.
 */
DaQOCloud.controller('unitInfoModalCtrl',['$scope','$http','$rootScope','$filter','$location','$modalInstance','unitManageService','paramObj',
    function($scope, $http, $rootScope,$filter,$location,$modalInstance,unitManageService,paramObj) {

        $scope.modalUnitInfo = paramObj;
        $scope.modalPjList = [];
        unitManageService.getProjectList().then(function (result) {
            if($scope.modalUnitInfo.intDBPkPjinfoid != undefined){
                $scope.selectedPj = $scope.modalUnitInfo.intDBPkPjinfoid+"";
                $scope.modalPjList.push(angular.copy($filter('filter')(result,{intDBPkPjinfoid:$scope.modalUnitInfo.intDBPkPjinfoid})[0]));
            }else{
                $scope.modalPjList = result;
                $scope.selectedPj = $scope.modalPjList[0].intDBPkPjinfoid+"";$scope.modalPjList
            }
            $scope.$apply($scope.modalPjList);
            $scope.$apply($scope.selectedPj);
        })

        //$("#pjList option[value="+$scope.modalUnitInfo.intDBPkPjinfoid+"]").attr("selected",'selected')
        $scope.cancel = function(){
            $scope.modalUnitInfo = paramObj;
            $modalInstance.dismiss('cancel'); // 退出
        };


        $scope.modalSubmit = function () {
            $scope.modalUnitInfo.strUnitNote = $("#strUnitNote").val();
            $scope.modalUnitInfo.strUnitName = $("#strUnitName").val();
            $scope.modalUnitInfo.intDBPkPjinfoid = $scope.selectedPj;
            $modalInstance.close($scope.modalUnitInfo)
        }

}]);