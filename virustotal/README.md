# VirusTotal

## description

```
I made shitty sandbox for monitor virus...
can you escape sandbox in blind? haha

flag is in `./flag` with root

http://virustotal.ap-northeast-2.elasticbeanstalk.com/
```

## Solution
1. make http request wrapping binary
2. get temporary AWS IAM credential(which setted to S3Access permission) with via ssrf to `GET http://169.254.169.254/latest/meta-data/iam/security-credentials/aws-elasticbeanstalk-ec2-role/`
3. get source with real flag in s3 bucket(aws eb automatically upload source in here by default)
