/**
 * Created by lojoso on 2017/1/22.
 */

DaQOCloud.controller('userInfoModalCtrl',['$scope','$http','$rootScope','$filter','$location',
    '$modalInstance','userInfoMangeService','paramObj',function($scope, $http, $rootScope,$filter,$location,$modalInstance,userInfoMangeService,paramObj) {

        $scope.SourcemodalUser = {};
        $scope.SourcemodalAllPjInfo = {};
        $scope.SourcemodalAllImei = {};
        $scope.modalUser = {};
        $scope.modalAllPjInfo = {};
        $scope.modalAllImei = {};
        if(paramObj.adminid != undefined){
            userInfoMangeService.getUser(paramObj.adminid,paramObj.pwd,paramObj.targetid).then(function (result) {
                $scope.modalUser = angular.copy(result);
                $("#strAuthority input[name=radio][value="+$scope.modalUser.strAuthority+"]").attr("checked",'checked')
                $scope.SourcemodalUser = result;
                $scope.$apply($scope.modalUser);
            })
        }

        userInfoMangeService.getAllPjInfo().then(function (result) {
            $scope.modalAllPjInfo = angular.copy(result);
            $scope.SourcemodalAllPjInfo = result;
            $scope.$apply($scope.modalAllPjInfo);
        })

        userInfoMangeService.getAllImei().then(function (result) {
            $scope.modalAllImei = angular.copy(result);
            $scope.SourcemodalAllImei = result;
            $scope.$apply($scope.modalAllImei);
        })

        $(function() {

        });

        $scope.deletePj = function(pjid){
            $scope.modalUser.pjList.forEach(function (e,i) {
                if(e.intDBPkPjinfoid == pjid){
                    $scope.modalUser.pjList.splice(i,1);
                }
            })
        }

        $scope.deleteImei = function(imeiid){
            $scope.modalUser.imeiList.forEach(function (e,i) {
                if(e.intImeiId == imeiid){
                    $scope.modalUser.imeiList.splice(i,1);
                }
            })
        }

        $scope.addPj = function(pjid){
            if($scope.modalUser.pjList == undefined) {
                $scope.modalUser.pjList = [];
                $scope.SourcemodalAllPjInfo.forEach(function (e, i) {
                    if (e.intDBPkPjinfoid == pjid) {
                        $scope.modalUser.pjList.push(e);
                    }
                })
            }else{
                if($filter('filter')($scope.modalUser.pjList,{intDBPkPjinfoid:pjid}).length == 0){
                $scope.SourcemodalAllPjInfo.forEach(function (e,i) {
                    if(e.intDBPkPjinfoid == pjid){
                        $scope.modalUser.pjList.push(e);
                    }
                })
            }
            }
        }

        $scope.addImei = function(imeiid){
            if($scope.modalUser.imeiList == undefined){
                $scope.modalUser.imeiList = [];
                $scope.SourcemodalAllImei.forEach(function (e, i) {
                    if (e.intImeiId == imeiid) {
                        $scope.modalUser.imeiList.push(e);
                    }
                })
            }else{
                if($filter('filter')($scope.modalUser.imeiList,{intImeiId:imeiid}).length == 0) {
                    $scope.SourcemodalAllImei.forEach(function (e, i) {
                        if (e.intImeiId == imeiid) {
                            $scope.modalUser.imeiList.push(e);
                        }
                    })
                }
            }
        }

        $scope.cancel = function(){
            $scope.modalUser = angular.copy($scope.SourcemodalUser);
            $scope.modalAllPjInfo = angular.copy($scope.SourcemodalAllPjInfo);
            $scope.modalAllImei = angular.copy($scope.SourcemodalAllImei);
            $modalInstance.dismiss('cancel'); // 退出
        };
        
        $scope.modalSubmit = function () {
            $scope.modalUser.strUsername = $('#strUsername').val();
            $scope.modalUser.strPassword = $('#strPassword').val();
            $scope.modalUser.strName = $('#strName').val();
            $scope.modalUser.strJob = $('#strJob').val();
            $scope.modalUser.strTelephone = $('#strTelephone').val();
            $scope.modalUser.strEmail = $('#strEmail').val();
            $scope.modalUser.strAuthority = $('#strAuthority input[name="radio"]:checked ').val();
            $modalInstance.close($scope.modalUser)
        }

}]);
