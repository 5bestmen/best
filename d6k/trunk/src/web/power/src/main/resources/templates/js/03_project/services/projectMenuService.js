/**
 * Created by reed on 2017/1/5.
 */
DaQOCloud.factory('projectMenuService',function ($http) {
    return {
        getPjDeviceInfo : function (userId) {
            /*$http({
                url:'rul',
                method:'get',
                params:{'_':new date()}
            }).then(function (data) {

            },function (status) {

            })*/
            return $http.get('http://172.16.42.51:8080http://172.16.42.37:8080/DaQOCloudSystem/ProjectManage/getProjectInfo?userid='+userId).then(function (data) {
                return data.data;
            },function (data, status) {
                alert('能不能长点心啊！');
            })
        }
    }
})