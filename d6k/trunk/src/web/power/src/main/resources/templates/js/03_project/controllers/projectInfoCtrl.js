/**
 * Created by reed on 2017/1/6.
 */
DaQOCloud.controller('projectInfoCtrl',['$scope','$stateParams','$state','$filter',function ($scope, $stateParams, $state, $filter) {
    //
    $scope.projectInfo = $filter('filter')($scope.pjInfoList,{'intDBPkPjinfoid':$stateParams.projectId})[0];
}]);