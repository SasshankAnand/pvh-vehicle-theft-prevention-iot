import json
import boto3
import base64

s3 = boto3.client('s3')
ses = boto3.client('ses', region_name='ap-south-1')

BUCKET_NAME = "vehicle-security-surveillance-evidence"
ALERT_EMAIL = "owner@example.com"
SOURCE_EMAIL = "alerts@autosecure-iot.com"

def lambda_handler(event, context):
    """
    Handles payload from ESP32 containing mode, GPS coordinates, and base64 image proof.
    """
    mode = event.get("mode", "THEFT")
    latitude = event.get("lat", 12.9728)
    longitude = event.get("lon", 79.1625)
    image_base64 = event.get("image", "")

    filename = f"evidence_{mode}_{event.get('timestamp', '0')}.jpg"
    
    # Store evidence image in S3 bucket
    if image_base64:
        image_bytes = base64.b64decode(image_base64)
        s3.put_object(
            Bucket=BUCKET_NAME,
            Key=filename,
            Body=image_bytes,
            ContentType='image/jpeg'
        )

    # Compose notification message
    if mode == "PVH":
        subject = "URGENT: Child Heatstroke (PVH) Alert Triggered!"
        body = f"Motion detected inside locked vehicle.\nVentilation actuated.\nLocation: https://maps.google.com/?q={latitude},{longitude}"
    else:
        subject = "SECURITY ALERT: Vehicle Theft / Break-in Detected!"
        body = f"Shock/Vibration event recorded.\nEvidence stored in S3 ({filename}).\nLocation: https://maps.google.com/?q={latitude},{longitude}"

    # Send notification via AWS SES
    ses.send_email(
        Source=SOURCE_EMAIL,
        Destination={'ToAddresses': [ALERT_EMAIL]},
        Message={
            'Subject': {'Data': subject},
            'Body': {'Text': {'Data': body}}
        }
    )

    return {
        'statusCode': 200,
        'body': json.dumps({'status': 'Alert dispatched successfully'})
    }
