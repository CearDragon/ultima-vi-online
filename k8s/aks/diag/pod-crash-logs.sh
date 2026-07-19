#!/bin/bash
kubectl config use-context k8s-ultimasixonline

kubectl -n ultima-dev get pods -l app=u6o-host -o wide
kubectl -n ultima-dev describe pod <pod-name>
kubectl -n ultima-dev logs <pod-name> --previous --timestamps
kubectl -n ultima-dev get events --sort-by=.metadata.creationTimestamp | Select-Object -Last 80

kubectl -n ultima-dev exec <pod-name> -- cat /proc/1/limits
kubectl -n ultima-dev exec <pod-name> -- cat /sys/fs/cgroup/memory.max
kubectl -n ultima-dev exec <pod-name> -- cat /sys/fs/cgroup/memory.current
kubectl -n ultima-dev exec <pod-name> -- cat /sys/fs/cgroup/memory.events

kubectl get nodes -o wide
kubectl describe node <node-name>
kubectl -n ultima-dev get pod <pod-name> -o jsonpath="{.status.containerStatuses[0].lastState.terminated.reason}{'\n'}{.status.containerStatuses[0].lastState.terminated.exitCode}{'\n'}{.status.containerStatuses[0].lastState.terminated.message}{'\n'}"