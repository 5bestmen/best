/**
 * Created by reed on 2016/11/23.
 */

DaQOCloud.controller('loginCtrl',['$scope','$stateParams','$state','loginService',function ($scope, $stateParams, $state,loginService) {

    $(function () {
        $('body').removeClass();
        $('body').addClass('body');

    })

    $scope.login = function() {
        /*var userName = $('#userIdInput').val(),
            passWord = $('#passwordInput').val();
        loginService.loginMessage(userName,passWord).then(function (result) {
            if(result.intStatus == 0){
                $scope.logininfo.pjID =result.lstPjInfo;
                $scope.logininfo.userInfoID =result.userInfo;
                $state.go('run');
            }else if(result.intStatus == 1){
                alert('登录失败，用户名错误！');
            }else{
                alert('登录失败，密码错误！');
            }
            $state.go('run');
        });*/

        console.info('home prev');
        $state.go('home');
    }

    $scope.rememberPassWord = function () {
        var src = $('.unselected').attr("src");
        if(src == "img/unselected.png"){
            $('.unselected').attr("src","img/selected.png");
        }else{
            $('.unselected').attr("src","img/unselected.png");
        }
    }

}]);
