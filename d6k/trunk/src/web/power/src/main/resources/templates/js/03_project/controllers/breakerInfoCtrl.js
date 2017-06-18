/**
 * Created by reed on 2017/1/6.
 */
DaQOCloud.controller('breakerInfoCtrl',['$scope','$stateParams','$state','$filter','$rootScope',function ($scope, $stateParams, $state, $filter, $rootScope) {
    //
    $scope.breakerInfo = $filter('filter')(
        $filter('filter')($scope.pjInfoList,{'intDBPkPjinfoid':$stateParams.projectId})[0].lstDeviceInfo,
        {'intDeviceid':$stateParams.breakerId}
    )[0];

}]);